import xml.etree.ElementTree as ET

def parse_xml(xml_bytes, tag):
    root = ET.fromstring(xml_bytes)
    return [
        {elem.tag: elem.text for elem in row}
        for row in root.findall(f".//{tag}")
    ]
