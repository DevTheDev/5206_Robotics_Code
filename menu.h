/**
 * holds the data for a menu
 */
typedef struct {
	int itemCount;//the number of items on the menu
	int selected;//the item number curent selection
	string itemNames[8];//the names of the items to display
	/*robotc does not support function pointers, so you must create a custom activaton function for each menu*///void (*actions)();//the functions to call when each menu item is selected
	string infos[8];//the strings of information to display, must update manually
} Menu;

/**
 * clears the NXT screen
 */
void clearScreen () {
  eraseDisplay(); //Clear the NXT screen
  bDisplayDiagnostics = false; //Takes control away from FCS
  bNxtLCDStatusDisplay = false; //Takes control away from NXT firmware
  wait1Msec(100);
}

/**
 * Displays the menu on the nxt screen
 * menu: the menu to display
 */
void displayMenu(Menu menu) {
	for (int i = 0; i < menu.itemCount; i++) {
		nxtDisplayCenteredTextLine(i, (i == menu.selected) ? ">%s< %s" : " %s %s", menu.itemNames[i], menu.infos[i]);
	}
}

/**
 * Selects the previous menu item
 * menu: the menu to modify
 */
void selectPrev (Menu menu) {
	// Add item count so selected stays non-negative
	menu.selected = (menu.selected - 1 + menu.itemCount) % menu.itemCount;
}

/**
 * Selects the next menu item
 * menu: the menu to modify
 */
void selectNext(Menu menu) {
	menu.selected = (menu.selected + 1) % menu.itemCount;
}

int prevButton = -1; // Global var? TODO: Delete me// Where do I put this; the NXT buttons are global, and robot c does not have namespaces?

/**
 * Must be run in a loop to use button checking functions
 */
void updateButtons() {
	if (prevButton != nNxtButtonPressed) {
		ClearTimer(T1);
	}
	prevButton = nNxtButtonPressed;
}

/**
 * Returns if button has been pressed
 * button: the button being tested
 */
bool pressed(int button) {
	if (prevButton != button && nNxtButtonPressed == button) {
		return true;
	}
	return false;
}

/**
 * Returns true if button has been released
 * button: the button being tested
 */
bool released(int button) {
	if (prevButton == button && nNxtButtonPressed != button) {
		return true;
	}
	return false;
}

/**
 * Returns if button has been held for time
 * button: the button being tested
 * holdTime: the time required for the button to have been held
 */
bool held(int button, int holdTime) {
	if (nNxtButtonPressed == button && time1[T1] > holdTime) {
		return true;
	}
	return false;
}
