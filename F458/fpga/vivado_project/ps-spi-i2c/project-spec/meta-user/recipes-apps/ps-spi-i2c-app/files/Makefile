APP = ps-spi-i2c-app

# Add any other object files to this list below
APP_OBJS = ps-spi-i2c-app.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)

