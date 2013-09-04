/*
 * boblight
 * Copyright (C) Bob  2009 
 * 
 * boblight is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * boblight is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define BOBLIGHT_DLOPEN
#include <libboblight/boblight.h>

#include <iostream>
#include <string>
#include <signal.h>
#include <unistd.h>

#include "hsvcolor.h"

using namespace std;

int  Run();
void SignalHandler(int signum);

volatile bool stop = false;


void PrintError(const std::string& error)
{
  std::cerr << "ERROR: " << error << "\n";
}


int main(int argc, char *argv[])
{
  //load the boblight lib, if it fails we get a char* from dlerror()
  char* boblight_error = boblight_loadlibrary(NULL);
  if (boblight_error)
  {
    PrintError(boblight_error);
    return 1;
  }

  //set up signal handlers
  signal(SIGTERM, SignalHandler);
  signal(SIGINT, SignalHandler);

  //keep running until we want to quit
  return Run();
}

int Run()
{
	int rgb[3];
	HSVColor hsv = HSVColor();
	
  while(!stop)
  {
    //init boblight
    void* boblight = boblight_init();

    cout << "Connecting to boblightd\n";
    
    //try to connect, if we can't then bitch to stderr and destroy boblight
    if (!boblight_connect(boblight, "127.0.0.1", -1, 5000000) ||
        !boblight_setpriority(boblight, 128))
    {
      PrintError(boblight_geterror(boblight));
      cout << "Waiting 10 seconds before trying again\n";
      boblight_destroy(boblight);
      sleep(10);
      continue;
    }

    cout << "Connection to boblightd opened\n";



    //keep checking the connection to boblightd every 10 seconds, if it breaks we try to connect again
    while(!stop)
    {
		 hsv.ToRGB(rgb);
		//set all lights to the color we want and send it
		boblight_addpixel(boblight, -1, rgb);
		if (!boblight_sendrgb(boblight, 1, NULL)) //some error happened, probably connection broken, so bitch and try again
		{
		  PrintError(boblight_geterror(boblight));
		  boblight_destroy(boblight);
		  continue;
		}
		hsv.NextHue();
      usleep(50000);
    }

    boblight_destroy(boblight);
  }

  cout << "Exiting\n";
  
  return 0;
}

void SignalHandler(int signum)
{
  if (signum == SIGTERM)
  {
    cout << "caught SIGTERM\n";
    stop = true;
  }
  else if (signum == SIGINT)
  {
    cout << "caught SIGINT\n";
    stop = true;
  }
}
