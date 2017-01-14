from sys import argv
import xml.etree.ElementTree as etree

inc_file = "obdemo.inc"
subdir = argv[1]

tree = etree.parse("pirddl.xml") 
root = tree.getroot()

for device in root:
    if device.attrib["name"] == "PIR":
        if device[0].attrib["type"] == "gpio" and device[0].attrib["mode"] == "in":
            mode         = "1"
            dtype        = "GPIO"
            PIR_BANK     = device[0][0].text
            PIR_PIN      = device[0][1].text
            pred         = device[1].attrib["red"]
            pgreen       = device[1].attrib["green"]
            pblue        = device[1].attrib["blue"]
            ared         = device[2].attrib["red"]
            agreen       = device[2].attrib["green"]
            ablue        = device[2].attrib["blue"]
            time         = device[3].text

presentColor = (int(pred) << 16) | (int(pgreen) << 8) | (int(pblue))
absentColor  = (int(ared) << 16) | (int(agreen) << 8) | (int(ablue))

code = """
        uint8   pirval;
        control(GPIO, """+PIR_BANK+""", """+PIR_PIN+""", """+mode+""");
	while (TRUE) {
            read("""+dtype+""", &pirval, ("""+PIR_BANK+""" << 16) | """+PIR_PIN+""");
            if(pirval == 0) {
                set_rgb("""+str(absentColor)+""");
            } else {
                set_rgb("""+str(presentColor)+""");
              }
            sleep("""+time+""");
	}
       """

with open(subdir+"/"+inc_file, 'w') as fout:
    fout.write(code)

