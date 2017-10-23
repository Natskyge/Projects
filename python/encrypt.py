alphabet = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']
clear_text = input("Enter sentence: ")
keys = {'1': "200591",
        '2': "854479"}

key1_list = map(int, keys['1'])
key2_list = map(int, keys['2'])


def encrypt(clear_text):
    state1 = 0
    state2 = 0
    text = list(clear_text)
    output_text = []
    for i in range(len(text)):
        if text[i] == ' ':
            output_text.append(' ')
            continue
        new_index = (alphabet.index(text[i])
                     + key1_list[state1]
                     + key2_list[state2]) % len(alphabet)
        output_text.append(alphabet[new_index])
        state1 = (state1 + 1) % 6
        state2 = (state1 + key1_list[state1]) % 6
    return ''.join(output_text)


print(encrypt(clear_text))
print("\n")


def decrypt(encrypted_text):
    state1 = 0
    state2 = 0
    text = list(encrypted_text)
    output_text = []
    for i in range(len(text)):
        if text[i] == ' ':
            output_text.append(' ')
            continue
        new_index = (alphabet.index(text[i])
                     - key1_list[state1]
                     - key2_list[state2]) % len(alphabet)
        output_text.append(alphabet[new_index])
        state1 = (state1 + 1) % 6
        state2 = (state1 + key1_list[state1]) % 6
    return ''.join(output_text)


print(decrypt(encrypt(clear_text)))
