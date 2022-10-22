# bg-gradski-prevoz // Belgrade Public Transport Simple Route Gen

## ***C Programming App Showcase - Belgrade Public Transport Simple Route Gen // The base for Python web app***

This simple **console app** works with static(already given)[^1] data represented as text files made of stations:
  
  `NumOfStation;NameOfStation;LatitudeCoord;LongitudeCoord;ZoneOfTransport` as one line in file.
  
  For example: `7!Pristaniste!44.8189915!20.4495147!1`, a line in file `2_dirA.txt`, **represents station 'Pristaniste' with number of station #7 of Line 2 in direction A**
  
  Every transport line is represented in 2 files: direction A and direction B.
  
  ## How to configure the app
  
  After cloning the repo with `git clone repo_path`, lines from directory **linije/** should be copied alongside with Source.cpp because of relative path of files. The output text file should also be made before use (just a plain text file with some name, such as output.txt)
  
  ## How to use the app
  
  Functionality of this app is behind the main functions arguments[^2]. This app was developed in MS Visual Studio, so editing command line arguments won't be same as editing in different IDEs:
  - Right click on Project_Name in Solution Explorer -> Properties
  - Find 'Debugging' and open
  - On the left side of window is Command-line arguments field, where data should be put in this particular way:
  
  `Output_file x1 y1 x2 y2 line1 line2 ... lineN`, where Output_file is name of output text file where results are displayed, x and y are coordinates of start and end stations and line1, ... lineN are N different transport lines that User want to use.
  
For example `output.txt 44.8189915 20.4495147 43.8129315 21.2494517 2 74 24`
  
## Results

Expected result of the app is visible in the **Output file** set as the first argument in Command-line arguments: list of all stations for each specified line that user has to go through in order to get to the end point(x2 y2 coords).

If we refer to the previous example, our Output file would have all the stations for lines 2, 74 and 24

# Future changes:exclamation:

Changes expected to be done in near future:
- **Make dynamic version of the app but in vanilla Python or some specific Python framework such as django**
  - The point is to make this app as a good base for future development, especially when we talk about web
  - Initally it's important to migrate from console to graphical view
- Add another way of inserting data
  - Sice Console-line arguments are outdated way of data communication and the project is targeting migration to web graphical view
  
  [^1]: Dynamic version under construction :smile:
  [^2]: In C programming(as well as in some other C-like langs), main function has different forms. In this case, main is defined like this: int main(int argc, char** argv)
