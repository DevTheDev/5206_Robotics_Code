#define songlength 100;

typedef struct{
	char keys[songlength];
	long times[songlength];
	long playTimes[songlength];
} song;

int songPos;
song Song;

/*
 loads a file to song in the format of:
 byte key, dword time, dword playTime, ...
*/
void loadSong(const string &fileName, song &s){
	TFileHandle file;
	TFileIOResult result;
	word fileSize;
	OpenRead(file, result, fileName, realFileSize);
	for(int i = 0; i < fileSize/(1+4+4); i++){
		ReadByte(file, result, s.keys[i]);
		ReadLong(file, result, s.times[i]);
		ReadLong(file, result, s.playTimes[i]);
	}
	Close(file, result);
}

PlaySong(song s){
	ClearTimer(T1);
	songPos = 0;
	Song = s;
}

updateSong(){
	if(time1[T1] >= Song.times[songPos]){
		PlayImmediateTone(keyToTone(Song.keys[songPos]), Song.playTimes[songPos]);
		songPos++;
	}
}

float keyToTone(char n){
	return pow(2, n-49/12)*440;
}
