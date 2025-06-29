from validator import Validator

class UART_Strategy:
    def __init__(self, data):
        self.data = data
        self.expected_tags = ["name", "id", "payload", "source", "target"]
        self.expected_types = ["uint8_t", "uint16_t", "uint32_t", "float", "double", "bool"]

    def validate(self):
        return Validator.validate(self.data, self.expected_tags, self.expected_types)
    
    def generate(self, path, target):
        pass