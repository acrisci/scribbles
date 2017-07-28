#include <stdio.h>
#include <wayland-client.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include "shared/os-compatibility.h"

struct client_state {
	struct wl_compositor *compositor;
	struct wl_shm *shm;
	struct wl_shell *shell;

	int width, height;

	void *data;
	struct wl_buffer *buffer;
};

void global_add(void *our_data,
				struct wl_registry *registry,
				uint32_t name,
				const char *interface,
				uint32_t version) {
	printf("added a global: %s\n", interface);
	struct client_state *state = our_data;
	if (strcmp(interface, "wl_compositor") == 0) {
		state->compositor =
				wl_registry_bind(registry, name,
								 &wl_compositor_interface, version);
	} else if (strcmp(interface, "wl_shm") == 0) {
		state->shm = wl_registry_bind(registry, name,
									  &wl_shm_interface, version);
	} else if (strcmp(interface, "wl_shell") == 0) {
		state->shell = wl_registry_bind(registry, name,
										&wl_shell_interface, version);
	}
}

void global_remove(void *our_data,
				   struct wl_registry *registry,
				   uint32_t name) {
	printf("removed a global\n");
}

struct wl_registry_listener registry_listener = {
		.global = global_add,
		.global_remove = global_remove
};


static void
handle_ping(void *data, struct wl_shell_surface *shell_surface,
			uint32_t serial)
{
	wl_shell_surface_pong(shell_surface, serial);
}

static void
handle_configure(void *data, struct wl_shell_surface *shell_surface,
				 uint32_t edges, int32_t width, int32_t height)
{
}

static void
handle_popup_done(void *data, struct wl_shell_surface *shell_surface)
{
}

static const struct wl_shell_surface_listener shell_surface_listener = {
		handle_ping,
		handle_configure,
		handle_popup_done
};

static void
create_shm_buffer(struct client_state *state)
{
	struct wl_shm_pool *pool;
	int fd, size, stride;

	stride = state->width * 4;
	size = stride * state->height;

	fd = os_create_anonymous_file(size);
	if (fd < 0) {
		fprintf(stderr, "creating a buffer file for %d B failed: %m\n",
				size);
		exit(1);
	}

	state->data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (state->data == MAP_FAILED) {
		fprintf(stderr, "mmap failed: %m\n");
		close(fd);
		exit(1);
	}

	pool = wl_shm_create_pool(state->shm, fd, size);
	state->buffer =
			wl_shm_pool_create_buffer(pool, 0,
									  state->width, state->height, stride,
									  WL_SHM_FORMAT_ARGB8888);
	wl_shm_pool_destroy(pool);

	close(fd);
}

int main() {
	struct wl_display *display = wl_display_connect("wayland-0");
	assert(display);

	struct wl_registry *registry = wl_display_get_registry(display);
	assert(registry);

	struct client_state *state = malloc(sizeof(state));
	state->width = 400;
	state->height = 600;

	wl_registry_add_listener(registry, &registry_listener, state);
	wl_display_dispatch(display);

	struct wl_surface *surface = wl_compositor_create_surface(state->compositor);
	assert(surface);

	struct wl_shell_surface *shell_surface = wl_shell_get_shell_surface(state->shell, surface);
	assert(shell_surface);

	wl_shell_surface_set_toplevel(shell_surface);

	create_shm_buffer(state);

	wl_shell_surface_set_title(shell_surface, "hello wayland");
	wl_shell_surface_add_listener(shell_surface, &shell_surface_listener, state);


	memset(state->data, 64, state->width * state->height * 4);
	wl_surface_attach(surface, state->buffer, 0, 0);
	wl_surface_damage(surface, 0, 0, state->width, state->height);
	wl_surface_commit(surface);

	int ret = 0;
	while (ret != -1)
		ret = wl_display_dispatch(display);

	return 0;
}