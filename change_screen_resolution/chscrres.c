#include <windows.h>
#include <stdio.h>

struct DDList {
	DISPLAY_DEVICE	Device;
	struct DDList	*Next;
};
static void DDList_Build(struct DDList **ppDD);
static void DDList_Clean(struct DDList **ppDD);
static BOOL DDList_Pop(struct DDList **ppDD, DISPLAY_DEVICE	*device);

static BOOL getResolution(int argc, char **argv, int *width, int *height);

int main(int argc, char **argv) {

	DISPLAY_DEVICE	device		= {0};
	struct DDList	*ddList		= NULL;
	DEVMODE			deviceMode	= {0};
	int				newWidth	= 0;
	int				newHeight	= 0;
	int				actualWidth	= 0;
	int				actualHeight= 0;

	if (!getResolution(argc, argv, &newWidth, &newHeight)) {
		return 1;
	}

	//	build display devices list
	DDList_Build(&ddList);

	//	get the first display device, if any (...); 
	//	decision is with user, or relying on other considerations
	device.cb = sizeof(device);

//	char buffer[32];
//	sprintf(buffer, "%d", device.cb);
//	MessageBox(NULL, buffer, "Display", MB_ICONINFORMATION);

	//MessageBox(0, TEXT("test"), 0, MB_OK);
	/*
	MessageBox(
		NULL,
		L"temp.txt already exists.\nDo you want to replace it?",
		L"Confirm Save As",
		MB_ICONEXCLAMATION | MB_YESNO
		);*/

	if(DDList_Pop(&ddList, &device)) {
		//	now change the display settings
			//	enumerate current (we can user NULL instead of device.DeviceName, 
			//	but this is more generic)
		deviceMode.dmSize			= sizeof(deviceMode);
		deviceMode.dmDriverExtra	= 0;
		
		//char buffer[8192];
		//sprintf(buffer, "DeviceName: %d", device.DeviceName);
	//	MessageBox(NULL, buffer, "Debug Message", MB_OK);

		if(EnumDisplaySettingsEx(device.DeviceName, ENUM_CURRENT_SETTINGS, &deviceMode, 0)) {			

			actualWidth = deviceMode.dmPelsWidth;
			actualHeight = deviceMode.dmPelsHeight;
			if((actualWidth != newWidth) || (actualHeight != newHeight)) {
				//	change requested
				deviceMode.dmPelsWidth	= newWidth;
				deviceMode.dmPelsHeight	= newHeight;
				if(ChangeDisplaySettingsEx(device.DeviceName, &deviceMode,	NULL, 0, NULL) == DISP_CHANGE_SUCCESSFUL) {
					exit(0);
					//	broadcast change to system
					//SendMessage(HWND_BROADCAST, WM_DISPLAYCHANGE, (WPARAM)(deviceMode.dmBitsPerPel), MAKELPARAM(newWidth, newHeight));					
				}			
			}
		}
		
	}

	

	//	clear list
	DDList_Clean(&ddList);

	return 0;
}

static void DDList_Build(struct DDList **ppDD) {
	DWORD			deviceNum	= 0;
	DISPLAY_DEVICE	device		= {0};
	struct DDList	*ddList		= NULL;
	struct DDList	*dd			= NULL;
	struct DDList	*ddLastNode	= NULL;

	if(ppDD == NULL) {
		return;
	}
	*ppDD = NULL;

	//	build display devices list
	device.cb = sizeof(device);
	do {
		if(!EnumDisplayDevices(NULL, deviceNum, &device, 0)) {
			break;
		}
		if(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) {
			dd = (struct DDList *)malloc(sizeof(struct DDList));
			if(!dd) {
				break;
			}
			dd->Device	= device;
			dd->Next	= NULL;

			if(ddList) {
				ddLastNode->Next = dd;
				ddLastNode = dd;
			}
			else {
				ddList = dd;
				ddLastNode = ddList;
			}
		}
		deviceNum++;
	} while(1);

	*ppDD = ddList;
}

static void DDList_Clean(struct DDList **ppDD) {
	struct DDList	*ddList		= NULL;
	struct DDList	*dd			= NULL;

	if(ppDD == NULL) {
		return;
	}
	ddList = *ppDD;
	//	clear list
	while(ddList) {
		dd = ddList;
		ddList = ddList->Next;
		free(dd);
	}
}

static BOOL DDList_Pop(struct DDList **ppDD, DISPLAY_DEVICE	*device) {
	struct DDList *ddList = NULL;
	struct DDList *ddHead = NULL;
	size_t cbDD = sizeof(DISPLAY_DEVICE);

	if(ppDD == NULL) {
		return FALSE;
	}
	if(device == NULL) {
		return FALSE;
	}
	if(device->cb < cbDD) {
		return FALSE;
	}
	ddList = *ppDD;
	CopyMemory(device, &ddList->Device, cbDD);

	ddHead = ddList;
	ddList = ddList->Next;
	free(ddHead);

	*ppDD = ddList;
	return TRUE;
}

static BOOL getResolution(int argc, char **argv, int *width, int *height) {
	char *arg = NULL;
	int n = 0;

	if(width == NULL || height == NULL) {
		return FALSE;
	}
	if(argc != 3) {
		return FALSE;
	}
	//	width
	arg = argv[1];
	n = atoi(arg);
	if(n <= 0) {
		return FALSE;
	}
	*width = n;
	//	height
	arg = argv[2];
	n = atoi(arg);
	if(n <= 0) {
		return FALSE;
	}
	*height = n;

	return TRUE;
}
