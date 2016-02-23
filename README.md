# color-supplier

Working on another project I had a need of a smalll tool which could easily provide RGB color values over the STDOUT as soon as I pick the color on a palette.

### Usage
To start the app run the run.sh script. **Optionally** manual palette width and height values can be passed (the default ones are 800 and 512):
```
$ ./run.sh 1200 600
```

The app works the following way:
* Press mouse's primary button over any color available in the generated palette
* The app will throw an output in the format **"R: 104, G: 194, B: 255"**
* Move the mouse around the palette's area while keeping on pressing the button
* The app will be producing an output of the same format for each pixel hit until the button is pressed.

![preview](https://raw.githubusercontent.com/alex-ant/color-supplier/master/images/demo.png)

### The Palette
The palette is being dynamically generated using the following RGB pattern:

![preview](https://raw.githubusercontent.com/alex-ant/color-supplier/master/images/palette.png)

### Dependencies
* Gtk+ 3
* Cairo
