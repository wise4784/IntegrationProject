APP = ps-spi-app

# Add any other object files to this list below
APP_OBJS = ps-spi-app.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)

