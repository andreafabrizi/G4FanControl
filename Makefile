

g4fancontrol: src/g4fancontrol.c src/IOI2C.c src/g4fc_controller.c src/g4fc_interface.c src/utils.c
	gcc  -L/usr/lib  -framework Foundation -framework IOKit -o g4fancontrol src/g4fancontrol.c src/IOI2C.c src/g4fc_controller.c src/g4fc_interface.c src/utils.c

clean:
	rm -fr g4fancontrol
