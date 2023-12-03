
# start docker-compose
docker-compose:
	@cd docker/compose && docker-compose up -d

# build docker image
docker-build:
	@docker build \
    		-t ktx/$(APP_RELATIVE_PATH) \
        	-f ./docker/$(PARENT_RELATIVE_PATH)/Dockerfile \
        	--build-arg PARENT_RELATIVE_PATH=$(PARENT_RELATIVE_PATH) \
        	--build-arg APP_RELATIVE_PATH=$(APP_RELATIVE_PATH) \
        	.

# build docker cache image: ttc_tb_user
docker-ttc-tb-user:
	@make docker-build PARENT_RELATIVE_PATH=cache APP_RELATIVE_PATH=ttc_tb_user

# build docker log image: tlog
docker-tlog:
	@make docker-build PARENT_RELATIVE_PATH=log APP_RELATIVE_PATH=tlog

# show help
help:
	@echo ""
	@echo "Usage:"
	@echo " make [target]"
	@echo ""
	@echo 'Targets:'
	@awk '/^[a-zA-Z\-_0-9]+:/ { \
	helpMessage = match(lastLine, /^# (.*)/); \
		if (helpMessage) { \
			helpCommand = substr($$1, 0, index($$1, ":")-1); \
			helpMessage = substr(lastLine, RSTART + 2, RLENGTH); \
			printf "\033[36m%-22s\033[0m %s\n", helpCommand,helpMessage; \
		} \
	} \
	{ lastLine = $$0 }' $(MAKEFILE_LIST)

.DEFAULT_GOAL := help