ARG DOCKER_BASE=ubuntu:bionic
ARG DOCKER_PLATFORM=linux/i386

FROM --platform=${DOCKER_PLATFORM} ${DOCKER_BASE}

ARG PARENT_RELATIVE_PATH
ARG APP_RELATIVE_PATH

COPY ${PARENT_RELATIVE_PATH}/${APP_RELATIVE_PATH} /usr/src/ktxapp/${APP_RELATIVE_PATH}

WORKDIR /usr/src/ktxapp/${APP_RELATIVE_PATH}/bin

CMD exec /bin/sh -c "./ttcd.sh start | sleep infinity"