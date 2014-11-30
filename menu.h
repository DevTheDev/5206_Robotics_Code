#include "joystickdriver.c";

// NXT buttons
#define leftarrow 2
#define rightarrow 1
#define orangebutton 3
#define graybutton 0

unsigned int menu_position = 0;
unsigned int menu_size = 0;
//unsigned int menu_display_pos = 0;
//char menu_list[16][16];


int prevButton = -1; // Global var? TODO: Delete me// Where do I put this; the NXT buttons are global, and robot c does not have namespaces?

/**
 Must be run in a loop to use button checking functions
*/
void updateButtons() {
	if (prevButton != nNxtButtonPressed) {
		ClearTimer(T1);
	}
	prevButton = nNxtButtonPressed;
}

/**
Returns if button has been pressed
button: the button being tested
*/
bool pressed(int button) {
	if (prevButton != button && nNxtButtonPressed == button) {
		return true;
	}
	return false;
}

/**
Returns true if button has been released
button: the button being tested
*/
bool released(int button) {
	if (prevButton == button && nNxtButtonPressed != button) {
		return true;
	}
	return false;
}

/**
Returns if button has been held for time
button: the button being tested
holdTime: the time required for the button to have been held
*/
bool held(int button, int holdTime) {
	if (nNxtButtonPressed == button && time1[T1] > holdTime) {
		return true;
	}
	return false;
}

void clearScreen () {
	eraseDisplay(); //Clear the NXT screen
	disableDiagnosticsDisplay(); //Takes control away from FCS
	bNxtLCDStatusDisplay = false; //Takes control away from NXT firmware
	wait1Msec(100);
}

/*
Displays the menu on the nxt screen
menu: the menu to display
*/
/*void displayMenu() {
	for (int i = 0; i < menu.itemCount; i++) {
	nxtDisplayCenteredTextLine(i, (i == menu.selected) ? ">%s< %s" : " %s  %s", menu.itemNames[i], menu.infos[i]);
}*/

bool displayMenuItem(char * name){
#define item (menu_size == menu_position) ? ">%s<" : " %s ", name
	if(menu_size < 8){
	nxtDisplayCenteredTextLine(menu_size, item);
	}
	else{
		scrollText(item)
	}
	return (menu_size++ == menu_position && nNxtButtonPressed == orangebutton);
#undef item
}

/*
Selects the previous menu item
menu: the menu to modify
*/
void selectPrev () {
	// Add item count so selected stays non-negative
	menu_position = (menu_position - 1) % menu_size;
}

/**
* Selects the next menu item
* menu: the menu to modify
*/
void selectNext() {
	menu_position = (menu_position + 1) % menu_size;
}

void updateMenu(){
	menu_position = (menu_position+menu_size - pressed(leftarrow) + pressed(rightarrow)) % menu_size;
	updateButtons();
	menu_size = 0;

}
