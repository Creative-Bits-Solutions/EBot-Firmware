#include <Snowball.h>

File root;

// Timer based wave parser and player
Wav wav;

// Index of playing file
char playIndex = 0;
char playButton = 10;

char response[40] = {0};

// Is system playing a file
boolean bPlaying = false;


boolean getArguments(char *arguments) {
  
  int i,j=0;

  for(i=0;i<100;i++) {    

    if (Serial.available() > 0) {
      arguments[j] = Serial.read(); 
      if(arguments[j] == 10) {
        arguments[j] = 0;
        return true;
      }
      j++;      
    } else {
      delay(10);
    }    

}
    return false;  
}
void putResponse(char *response) {
  Serial.print(response);
  Serial.print('\n');
}

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  putResponse("Starting SD Player");
  
  if (!SD.begin(5)) {
    putResponse("ERROR:SD Not found!");
    return;
  }
  
  // Setup button for playback if required
  pinMode(playButton,INPUT);
  digitalWrite(playButton,HIGH); // pullup 
  
  putResponse("SD Started");
 
}

void loop()
{
  int i;
  char cCommand ;
  // nothing happens after setup finishes.
  
  // If user presses a button play a file 
  if(digitalRead(playButton) == 0 ) {    
    playNext();    
    while(digitalRead(playButton)== 0) {
      delay(100);
    }
  }
  
  // Did we recive a command from PC software 
  if(Serial.available() > 0) {
    cCommand = Serial.read();
    
    if(cCommand != '$') return;
    delay(10);
    cCommand = Serial.read();
    
    if( getArguments(response) )
    {
   
    // Is something playing then disable it 
    if(bPlaying) {
      wav.disable();
      bPlaying = false;
    }   
      

    // Process PC commands 
    switch(cCommand) {
      
      // Delete a file 
      case 'D':
          SD.remove(response);
          putResponse("SUCCESS:D");                                                          
          break;         
      
      // List files 
      case 'L':          
          root = SD.open("/");                    
          root.rewindDirectory(); 
          showFiles(root);  
          root.close();
          putResponse("SUCCESS:L");    
          break;
          
      //  Play file   
      case 'P':
          wav.play(response);    
          bPlaying = true;
          putResponse("SUCCESS:P");            
          break;
          
      // Check if Firmware is loaded
      case 'C':
          putResponse("SUCCESS:C");                
          break;
          
      // Upload a file
      case 'U':
          {
          unsigned char fBuffer[1024] = {0};
          
          putResponse("Deleting");
          
          // Remove if file exists           
          SD.remove(response);  
          
          putResponse("Writing");
                    
          // Wrtie file to SD Card          
          File wFile = SD.open(response,FILE_WRITE);
          if(wFile) {
            long size =0;
            int k = 0; 
            
            // second timeout 
            
            for(i=0;i<2000;i++) {
              
              while(Serial.available()>0) {
                fBuffer[k] = Serial.read(); 
                size++;
                if(k == 1023) {
                  Serial.write("[M");
                  wFile.write(fBuffer,1024);
                  Serial.write("]");
                  k = 0 ; 
                } else {
                  k++;
                }
                i=0;
              }              
               delay(1);
            }       
            
            putResponse("Timeout");
            if(k>0) {
                  Serial.write("[F");
                  wFile.write(fBuffer,k);
                  Serial.write("]");
            }    
            
            wFile.close();           
            putResponse("SUCCESS:C");  
            break;
          } 
          //break on 1 second timeout
          putResponse("ERROR:Cannot write");  
        }
          break;
    
      default: 
          putResponse("ERROR:Invalid Command");
          break;
    }
    } else {
        putResponse("ERROR:Invalid Command");
    }
    
    
  }


}

// List files in a directory 
void showFiles(File dir) {
   File entry;
   boolean first = true;
   while(true) {     
     entry =  dir.openNextFile();
     if (! entry) {       
       break;
     }     
     if(!first) {
       Serial.print("|");
     }     
     Serial.print(entry.name());
     Serial.print(",");
     Serial.print(entry.size(), DEC);     
     first = false;
     }
   Serial.print('\n');  
   entry.close();
}

// Play next files based on current index 
void playNext() {
     
   root = SD.open("/");                    
   root.rewindDirectory(); 
    
   File entry;
   int i=0;
   
   while(true) {     
     entry =  root.openNextFile();
     if (! entry) {       
       break;
     }     
//     Serial.print(entry.name());
     if(i==playIndex) {       
       entry.close();
       wav.play(entry.name());  
       playIndex++;
       return;
     }
     i++;
    }
   entry.close();
   playIndex=0;
}