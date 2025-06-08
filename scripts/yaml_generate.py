from os import name
from yaml import load, Loader
import argparse

from internal_strategy import Internal_Strategy
from uart_strategy import UART_Strategy
from can_strategy import CAN_Strategy

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