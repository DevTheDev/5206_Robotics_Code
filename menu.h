/**
 * TODO
 */
typedef struct {
	int itemCount;//the number of items on the menu
	int selected;//the item number curent selection
	string* itemNames;//the names of the items to display
	/*robotc does not support function pointers, so you must create a custom activaton function for each menu*///void (*actions)();//the functions to call when each menu item is selected
	string* infos;//the strings of information to display, must update manually
} Menu;

/**
 * TODO
 */
void clearScreen () {
  eraseDisplay(); //Clear the NXT screen
  bDisplayDiagnostics = false; //Takes control away from FCS
  bNxtLCDStatusDisplay = false; //Takes control away from NXT firmware
  wait1Msec(100);
}

/**
 * Displays the menu on the nxt screen
 * menu: TODO
 */
void displayMenu(Menu menu) {
	for (int i = 0; i < menu.itemCount; i++) {
		if (i == menu.selected) {
			nxtDisplayTextLine(i, ">#%s< %s", menu.itemNames[i], menu.infos[i]);
		}
		else {
			nxtDisplayTextLine(i, "#%s: %s", menu.itemNames[i], menu.infos[i]);
		}
	}
	return;
}

/**
 * Selects the previous menu item
 * menu: TODO
 */
void selectPrev (Menu menu) {
	// Add item count so selected stays non-negative
	menu.selected = (menu.selected - 1 + menu.itemCount) % menu.itemCount;
	return;
}

/**
 * Selects the next menu item
 * menu: TODO
 */
void selectNext(Menu menu) {
	menu.selected = (menu.selected + 1) % menu.itemCount;
	return;
}

int prevButton = -1; // Global var? TODO: Delete me

/**
 * Must be run in a loop to use button checking functions
 */
void updateButtons() {
	if (prevButton != nNxtButtonPressed) {
		ClearTimer(T1);
	}
	prevButton = nNxtButtonPressed;
	return;
}

/**
 * Returns if the button has been pressed
 * button: TODO
 */
bool pressed(int button) {
	if (prevButton != button && nNxtButtonPressed == button) {
		return true;
	}
	return false;
}

/**
 * Returns true if button has been released
 * button: TODO
 */
bool released(int button) {
	if (prevButton == button && nNxtButtonPressed != button) {
		return true;
	}
	return false;
}

/**
 * Returns if button has been held for time
 * button: TODO
 * time: TODO; CHANGE MY NAME: it is a special var in c
 */
bool held(int button, int time) {
	if (nNxtButtonPressed == button && time1[T1] > time) {
		return true;
	}
	return false;
}
