
IMAGE:=idesk-builder
PREVIEW_IMAGE:=idesk-preview

UUID:=$(shell id -u)
UGID:=$(shell id -g)
PERMS:=$(PERMS) --user $(UUID)
DOCKER:=docker

CMD:=$(DOCKER) run --rm -ti --tmpfs=/tmp --volume $(CURDIR)/..:/home/buser/idesk

# default target, builds idesk without installing it
idesk: $(IMAGE)
	@rm -fv src/idesk
	$(CMD) $(PERMS) localhost/$(IMAGE) sh -e -c \
		"autoreconf --install; ./configure --enable-svg --enable-libsn; make -j"
.PHONY: idesk

# starts a dockerized VNC server with password 'idesk'
preview: $(PREVIEW_IMAGE)
	$(CMD) --publish 5900:5900 localhost/$(PREVIEW_IMAGE)
.PHONY: preview

$(IMAGE):
	$(DOCKER) build --build-arg UUID=$(UUID) --build-arg UGID=$(UGID) -t localhost/$(IMAGE) -f Dockerfile .
.PHONY: $(IMAGE)

$(PREVIEW_IMAGE):
	$(DOCKER) build --build-arg UUID=$(UUID) --build-arg UGID=$(UGID) -t localhost/$(PREVIEW_IMAGE) -f Dockerfile.preview .
.PHONY: $(PREVIEW_IMAGE)

# shell to inspect build container
builder_shell: $(IMAGE)
	$(CMD) $(PERMS) --entrypoint /bin/bash localhost/$(IMAGE) -i
.PHONY: builder_shell

# shell to inspect preview container
preview_shell: $(IMAGE)
	$(CMD) $(PERMS) --entrypoint /bin/bash localhost/$(PREVIEW_IMAGE) -i
.PHONY: preview_shell
