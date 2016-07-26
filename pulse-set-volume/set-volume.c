#include <stdio.h>
#include <pulse/pulseaudio.h>
#include <glib.h>

static int index = 0;
static double percent = 0.3;

static pa_mainloop_api *mainloop_api;
static pa_context *context;
static pa_cvolume volume;

static void quit(int ret) {
    mainloop_api->quit(mainloop_api, ret);
}

static void context_drain_complete(pa_context *c, void *userdata) {
    pa_context_disconnect(c);
}

static void simple_callback(pa_context *c, int success, void *userdata) {
    if (!success) {
        printf("Failure: %s", pa_strerror(pa_context_errno(c)));
        quit(1);
        return;
    }

    pa_operation *o;

    if (!(o = pa_context_drain(context, context_drain_complete, NULL)))
        pa_context_disconnect(context);
    else
        pa_operation_unref(o);
}


static void get_sink_volume_callback(pa_context *c, const pa_sink_info *i, int is_last, void *userdata) {
    if (is_last < 0) {
        printf("failed to get sink information: %s\n", pa_strerror(pa_context_errno(c)));
        quit(1);
    }

    if (is_last)
        return;

    //const pa_cvolume cv = i->volume;

    pa_cvolume_set(&volume, i->channel_map.channels, PA_VOLUME_NORM * percent);

    //char s[PA_CVOLUME_SNPRINT_MAX];
    //printf("changing volume to:\n   %s\n", s);
    //pa_cvolume_snprint(s, sizeof(s), &volume);

    pa_operation_unref(pa_context_set_sink_volume_by_index(c, index, &volume, simple_callback, NULL));
}

static void context_state_callback(pa_context *c, void *userdata) {
    switch (pa_context_get_state(c)) {
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
            break;
        case PA_CONTEXT_READY:
            {
            pa_operation *o = pa_context_get_sink_info_by_index(c, index,  get_sink_volume_callback, NULL);

            if (o)
                pa_operation_unref(o);

            break;
            }

        case PA_CONTEXT_TERMINATED:
            quit(0);
            break;

        case PA_CONTEXT_FAILED:
        default:
            printf(("Connection failure: %s"), pa_strerror(pa_context_errno(c)));
            quit(1);
            break;
    }
}

int main(int argc, char *argv[]) {
    int ret = 1;
    pa_mainloop *m = NULL;

    if (argc < 3) {
        printf("usage: ./set-volume <SINK-IDX> <PERCENT>\n");
        return 0;
    }

    char *endptr;

    index = strtol(argv[1], &endptr, 0);

    if (endptr == argv[1]) {
        printf("couldn't parse sink index as integer: %s\n", argv[1]);
        return 1;
    }

    percent = strtod(argv[2], &endptr) / 100;

    if (endptr == argv[2]) {
        printf("couldn't parse percent as integer: %s\n", argv[2]);
        return 1;
    }
    
    if (!(m = pa_mainloop_new())) {
        printf("could not create main loop\n");
        return 1;
    }

    mainloop_api = pa_mainloop_get_api(m);

    context = NULL;
    
    if (!(context = pa_context_new(mainloop_api, NULL))) {
        printf("could not create new context\n");
        return 1;
    }

    pa_context_set_state_callback(context, context_state_callback, NULL);

    if (pa_context_connect(context, NULL, 0, NULL) < 0) {
        printf("could not connect to server\n");
        return 1;
    }

    if (pa_mainloop_run(m, &ret) < 0) {
        printf("could not run main loop\n");
        return 1;
    }

    return ret;
}
