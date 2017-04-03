FROM debian:8.7
RUN apt-get update && apt-get install -y --no-install-suggests --no-install-recommends \
    python3-pip \
    nginx
COPY container/nginx.conf /etc/nginx/nginx.conf
WORKDIR /app
VOLUME /app/data
COPY . /app
RUN pip3 install -r requirements.txt
CMD nginx && python3 ./manage.py runserver
