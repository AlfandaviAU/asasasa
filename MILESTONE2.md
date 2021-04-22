# Milestone 2

### loadFile

* Compile with ```./compile.sh```.
* Edit map.img with ```hexedit.img``` command. Fill with 70 bytes FF.
* Compile ```./dd.sh```.
* Compile the loadFile.c with ```gcc loadFile.c -o loadFile```.
* Load the file into img with sample test.txt file ```./loadFile test.txt test.txt```.
* Run the bochs with ```./bochs.sh```.
* Make sure all the scripts are given access to run. In case it has not been given any, do so with command ```chmod +x namafile.sh```.

### Shell Command
* ls: ```ls ```. (end it with a space)
* cat: ```cat namafile```.
* cd : No mkdir yet.
* ln: No link yet.