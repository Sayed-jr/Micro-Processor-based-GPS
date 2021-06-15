import gmplot
import serial

from serial import Serial

ser = serial.Serial(port = "COM4", baudrate=9600, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)
data = ser.readline(1000)
print(data)



latitude_list = [ 30.0358376, 30.007977, 30.06413064441]
longitude_list = [ 31.8701919, 31.048457, 31.27918807902]

gmap3 = gmplot.GoogleMapPlotter(30.0164945,
								31.03219179999999, 13)

# scatter method of map object
# scatter points on the google map
gmap3.scatter( latitude_list, longitude_list,
                              size = 40, marker = False )

# Plot method Draw a line in
# between given coordinates
gmap3.plot(latitude_list, longitude_list,
		'cornflowerblue', edge_width = 2.5)

gmap3.draw( "map13.html" )
