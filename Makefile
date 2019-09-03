
CC=gcc

MYFLAGS=
MYLIBS=

CFLAGS=-Og -g -Wall -Wextra -std=gnu99 -I./include $(MYFLAGS)
LIBS=-luser32 -lgdi32 -lopengl32 $(MYLIBS)
OBJS=win_platform.o render_mgr.o opengl_loading.o

main: main.o $(OBJS)
	$(CC) -static-libstdc++ -o $@ $^ $(LIBS)

main.o: main.c

win_platform.o: win_platform.c
render_mgr.o: render_mgr.c
opengl_loading.o: opengl_loading.c

clean:
	rm -f main.exe
	rm -f main.o $(OBJS)
