from email import header
from os import name
from yaml import load, Loader
import argparse

class Validator:
    def validate(data, expected_tags, expected_types):
        try:
            ids = []
            for message in data:
                Validator.check_whitespaces(message)
                Validator.check_message_tags(data[message].keys(), expected_tags)

                ids.append(data[message]["id"])
                payload_amount = len(data[message]["payload"])

                for payload_tag in data[message]["payload"]:
                    if(not payload_tag and payload_amount > 1):
                        raise Exception(f"If there is a payload in {message} message, there can be no null tag!")
                    if(payload_tag):
                        Validator.check_payload_datatype(payload_tag[list(payload_tag.keys())[0]], expected_types)
            
            Validator.check_list_uniqueness(ids, "ids")

            return True
        except Exception as error:
            print('ERROR | ' + error.args[0])
        
        return False

    def check_whitespaces(text: str):
        if text.count(" ") != 0:
            raise Exception(f"There should be no whitespaces in tag names! TAG_NAME: {text}")
    
    def check_message_tags(tags: list, expected_tags: list):
        for tag in tags:
            Validator.check_whitespaces(tag)
            if tag not in expected_tags:
                raise Exception(f"The {tag.upper()} tag is not expected for this type of message")
        if len(tags) != len(expected_tags):
            raise Exception("The amount of provided message tags does not match the expected amount!")
        
    def check_list_uniqueness(array: list, list_name: str):
        if len(array) > len(set(array)):
            raise Exception(f"All {list_name} should be UNIQUE!")
        
    def check_payload_datatype(type: str, known_types: list):
        if type not in known_types:
            raise Exception(f"{type} is an unknown type!")

        
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

class UART_Strategy:
    pass

class CAN_Strategy:
    pass

class YAML_Parser:
    def __init__(self):
        self.known_types = ["UART", "CAN", "Internal"]
        self.data = None
        self.strategy = None

    def parse(self, path):
        try:
            f = open(path, "r", encoding="utf-8")
            self.data = load(f, Loader=Loader)
            return True
        except FileNotFoundError:
            self.print_error("Provided file was NOT found!")
            return False
        
    def validate(self):
        try:
            assert(self.data)

            type = self.data['HEADER']['type']
            self.data.pop('HEADER')

            if type == "Internal":
                self.strategy = Internal_Strategy(self.data)
            elif type == "UART":
                self.strategy = UART_Strategy(self.data)
            elif type == "CAN":
                self.strategy = CAN_Strategy(self.data)
            else:
                raise KeyError()
            
            if not self.strategy.validate():
                raise Exception
            
            return True
        except KeyError:
            self.print_error("The HEADER part of provided messages config is improperly formatted!")
        except AssertionError:
            self.print_error("There is no data to be validated!")
        except Exception:
            self.print_error("Provided config was not successfully validated!")
        return False

    def generate(self, target_file) -> None:
        try:
            assert(self.strategy)
            self.strategy.generate(target_file)

            return True
        except AssertionError:
            self.print_error("There was no successful data validation done!")
        return False

    def print_error(self, message):
        print("ERROR |", message)

if __name__=="__main__":
    arg_parser = argparse.ArgumentParser(
                    prog='YAML messages generator',
                    description='Generate target specific messages from yaml files',
                    epilog='Author: Szymon Hrehorowicz')
    arg_parser.add_argument("config_file")
    arg_parser.add_argument("target_file")
    args = arg_parser.parse_args()

    yaml_parser = YAML_Parser()
    
    if(yaml_parser.parse(args.config_file)):
        if(yaml_parser.validate()):
            yaml_parser.generate(args.target_file)