from numpy.typing import NDArray
import numpy as np
import xml.etree.ElementTree as Et
from lxml import etree
import os

class GDMLWriter:

    def __init__(self,vertices: NDArray[tuple[float,float,float]], faces: NDArray[tuple[int,int,int]], solid_name, output_file) -> None:
        self.NS_XSI = "http://www.w3.org/2001/XMLSchema-instance"
        self.XSI_SCHEMA = "http://service-spi.web.cern.ch/service-spi/app/releases/GDML/schema/gdml.xsd"
        self.unit = "mm"
        self.schema_dir = "schema"
        gdml = self.generate_gdml(vertices, faces, solid_name)
        if not self.valid(gdml):
            raise ValueError("Stopped generation of gdml file, structure not valid!")
        self.write_gdml(gdml, output_file)

    def valid(self,gdml) -> bool:
        schema_path = os.path.join(self.schema_dir, "gdml.xsd")
        schema_doc = etree.parse(schema_path)
        schema = etree.XMLSchema(schema_doc)

        # Convert ElementTree Element → lxml Element
        gdml_bytes = Et.tostring(gdml, encoding="utf-8")
        gdml_lxml = etree.fromstring(gdml_bytes)

        valid = schema.validate(gdml_lxml)
        if valid:
            return True
        else:
            print("GDML schema validation failed")
            for err in schema.error_log:
                print(f"Line {err.line}: {err.message} (Domain: {err.domain_name})")
                # Optional: print the source line for context
                if err.path is not None:
                    print(f"XPath: {err.path}")
        return False

    @staticmethod
    def write_gdml(gdml, output_file:str) -> None:
        tree = Et.ElementTree(gdml)
        Et.indent(tree, space="  ", level=0)
        output_file = output_file + ".xml"
        tree.write(output_file, encoding="UTF-8", xml_declaration=True)

    def generate_gdml(self, vertices:NDArray[tuple[float,float,float]], faces: NDArray[tuple[int,int,int]], solid_name:str):
        Et.register_namespace("xsi", self.NS_XSI)
        gdml = Et.Element("gdml",
                          {f"{{{self.NS_XSI}}}noNamespaceSchemaLocation": self.XSI_SCHEMA})

        # --- <define> block ---
        define = Et.SubElement(gdml, "define")
        for i, (x, y, z) in enumerate(vertices):
            Et.SubElement(define, "position", name=f"v{i}", unit=self.unit, x=str(x), y=str(y), z=str(z))

        # --- <materials> block ---
        Et.SubElement(gdml, "materials")

        # --- <solids> block ---
        solids = Et.SubElement(gdml, "solids")
        tessellated = Et.SubElement(solids, "tessellated", aunit="degree", lunit=self.unit, name=solid_name)
        for i1, i2, i3 in faces:
            Et.SubElement(tessellated, "triangular",
                          vertex1=f"v{i1}", vertex2=f"v{i2}", vertex3=f"v{i3}",
                          type="ABSOLUTE")

        Et.SubElement(gdml, "structure")

        Et.SubElement(gdml, "userinfo")

        setup = Et.SubElement(gdml, "setup", name="tessellated_surface", version="1.0")
        Et.SubElement(setup, "world", ref="World")

        return gdml


def main():
    vertices = np.array([[0.0,1.0,0.0],[0.0,0.0,1.0],[0.0,0.0,0.0],[0.0,1.0,1.0]])
    faces = np.array([[0,1,2],[0,1,3]])
    GDMLWriter(vertices, faces, "test_solid", "test_gdml")


if __name__ == "__main__":
    main()