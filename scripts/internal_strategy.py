from validator import Validator

class Internal_Strategy(Validator):
    def __init__(self, data):
        self.data = data
        self.expected_tags = ["name", "id", "payload"]
        self.expected_types = ["uint8_t", "uint16_t", "uint32_t", "float", "double", "bool"]

    def validate(self):
        return Validator.validate(self.data, self.expected_tags, self.expected_types)

    def generate(self, path):
        content = f"""// THIS FILE WAS AUTO-GENERATED, DO NOT MODIFY\n\n"""
        content += "#pragma once\n\n"
        content += '#include "active_message.h"\n\n'
        content += 'enum Message_Id {\n'
        for message in self.data:
            content += r"   "
            content += message.upper()
            content += " = " + str(self.data[message]["id"]) + ",\n"
        content += "};\n\n"

        for message in self.data:
            formatted_name = '_'.join([s.capitalize() for s in message.split("_")])
            content += "using " + formatted_name + "_Message = Active::Message<Message_Id::" + message.upper()

            if(self.data[message]["payload"][0]):
                for element in self.data[message]["payload"]:
                    data_type = element[list(element.keys())[0]]
                    content += ", " + data_type

            content += ">;\n"

        with open(path, "w") as header_file:
            header_file.write(content)