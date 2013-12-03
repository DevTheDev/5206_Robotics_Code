typedef struct{
	int itemCount;//the number of items on the menu
	int selected;//the item number curent selection
	string itemNames[];//the names of the items to display
	void (*actions)()[];//the functions to call when each menu item is selected
	string (*infos)()[];//the functions for getting information to display
} Menu;


/**
	displays the menu on the nxt screen
*/
void displayMenu(Menu menu){
	for(int i = 0; i < menu.itemCount; i++){
		if(i == menu.selected){
			nxtDisplayTextLine(i, ">#%s< %s", menu.itemNames[i], infos[i]());
		}
		else{
			nxtDisplayTextLine(i, "#%s: %s", menu.itemNames[i], infos[i]());
		}
	}
	return;
}

/**
	activates the menus selected function
*/
void activate(Menu menu){
	menu.actions[menu.selected]();
	return;
}

/**
	selects the previous menu item
*/
void selectPrev(Menu menu){
	menu.selected = (menu.selected - 1 + itemCount) % itemCount;//add item count so selected stays non-negative
	return;
}
/**
	selects the next menu item
*/
void selectNext(Menu menu){
	menu.selected = (menu.selected + 1) % itemCount;
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
	return
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
