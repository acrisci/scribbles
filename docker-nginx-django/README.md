# Docker Nginx Django

It's a django app running in a docker container with an nginx proxy pass.

## Building and Running

```bash
docker build -t django-test .
docker run -d -p 8080:80 -v $(pwd)/data:/app/data django-test
curl localhost:8080
```
