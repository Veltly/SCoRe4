from numpy.typing import NDArray
import numpy as np
import xml.etree.ElementTree as Et
from lxml import etree

class GDMLWriter:
    def __init__(self,vertices: NDArray[tuple[float,float,float]], faces: NDArray[tuple[int,int,int]], solid_name, output_file) -> None:
        self.unit = "mm"
        self.scheme_link = "https://service-spi.web.cern.ch/service-spi/app/releases/GDML/schema/gdml.xsd"
        gdml = self.generate_gdml(vertices, faces, solid_name)
        if not self.valid(gdml):
            raise ValueError("GDML not valid")
        self.write_gdml(gdml, output_file)

    def valid(self,gdml) -> bool:
        #schema_doc = etree.parse(self.scheme_link)
        #schema = etree.XMLSchema(schema_doc)
        #return schema.validate(gdml)
        return True

    @staticmethod
    def write_gdml(gdml, output_file:str) -> None:
        tree = Et.ElementTree(gdml)
        Et.indent(tree, space="  ", level=0)
        output_file = output_file + ".xml"
        tree.write(output_file, encoding="UTF-8", xml_declaration=True)

    def generate_gdml(self, vertices:NDArray[tuple[float,float,float]], faces: NDArray[tuple[int,int,int]], solid_name:str):
        gdml = Et.Element("gdml")

        # --- <define> block ---
        define = Et.SubElement(gdml, "define")
        for i, (x, y, z) in enumerate(vertices):
            Et.SubElement(define, "position", name=f"v{i}", unit=self.unit, x=str(x), y=str(y), z=str(z))

        # --- <solids> block ---
        solids = Et.SubElement(gdml, "solids")
        tessellated = Et.SubElement(solids, "tessellated", aunit="degree", lunit=self.unit, name=solid_name)
        for i1, i2, i3 in faces:
            Et.SubElement(tessellated, "triangular",
                          vertex1=f"v{i1}", vertex2=f"v{i2}", vertex3=f"v{i3}",
                          type="ABSOLUTE")

        return gdml


def main():
    # vertices = np.array([[0.0,1.0,0.0],[0.0,0.0,1.0],[0.0,0.0,0.0],[0.0,1.0,1.0]])
    # faces = np.array([[0,1,2],[0,1,3]])
    # writer = GDMLWriter(vertices, faces, "test_solid", "test_gdml")
    schema_doc = etree.parse("http://service-spi.web.cern.ch/service-spi/app/releases/GDML/schema/gdml.xsd")

if __name__ == "__main__":
    main()