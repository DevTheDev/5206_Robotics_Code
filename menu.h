typedef struct{
	int itemCount;//the number of items on the menu
	int selected;//the item number curent selection
	string* itemNames;//the names of the items to display
	/*robotc does not support function pointers, so you must create a custom activaton function for each menu*///void (*actions)();//the functions to call when each menu item is selected
	string* infos;//the strings of information to display, must update manually
} Menu;

void clearScreen(){
  eraseDisplay(); //Clear the NXT screen
  bDisplayDiagnostics = false; //Takes control away from FCS
  bNxtLCDStatusDisplay = false; //Takes control away from NXT firmware
  wait1Msec(100);
}

/**
	displays the menu on the nxt screen
*/
void displayMenu(Menu menu){
	for(int i = 0; i < menu.itemCount; i++){
		if(i == menu.selected){
			nxtDisplayTextLine(i, ">#%s< %s", menu.itemNames[i], menu.infos[i]);
		}
		else{
			nxtDisplayTextLine(i, "#%s: %s", menu.itemNames[i], menu.infos[i]);
		}
	}
	return;
}

/**
	selects the previous menu item
*/
void selectPrev(Menu menu){
	menu.selected = (menu.selected - 1 + menu.itemCount) % menu.itemCount;//add item count so selected stays non-negative
	return;
}
/**
	selects the next menu item
*/
void selectNext(Menu menu){
	menu.selected = (menu.selected + 1) % menu.itemCount;
	return;
}

int prevButton = -1;

/**
	must be run in a loop to use button checking functions
*/
void updateButtons(){
	if(prevButton != nNxtButtonPressed){
		ClearTimer(T1);
	}
	prevButton = nNxtButtonPressed;
	return;
}

/**
	returns if the button has been pressed
*/
bool pressed(int button){
	if(prevButton != button && nNxtButtonPressed == button){
		return true;
	}
	return false;
}

/**
	returns true if button has been released
*/
bool released(int button){
	if(prevButton == button && nNxtButtonPressed != button){
		return true;
	}
	return false;
}

/**
	returns if button has been held for time
*/
bool held(int button, int time){
	if(nNxtButtonPressed == button && time1[T1] > time){
		return true;
	}
	return false;
}
