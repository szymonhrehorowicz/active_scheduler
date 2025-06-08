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