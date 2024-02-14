import hashlib

target_sha1 = "abc21d3f9d5d98ecae9f3d514475290171f2bbed"  # 替换为您的目标SHA-1值

start_value = 0x2111111519cb5fb6
end_value = 0xffffffffffffffff
cnt = 0
for current_value in range(start_value, end_value + 1):
    hex_string = hex(current_value)[2:]  # 转换为十六进制字符串，去掉前缀 '0x'
    # 计算SHA-1值
    hash_object = hashlib.sha1(bytes.fromhex(hex_string))
    current_sha1 = hash_object.hexdigest()
    cnt = 1 + cnt
    if cnt >= 10000000:
        with open("ApnounceDecrypt.txt","a") as file: 
                    file.write(hex(current_value)[2:])
                    file.write(" ")
                    file.write(current_sha1)
                    file.write("\n")
                    file.close()
        cnt=0
    if current_sha1 == target_sha1:
        with open("ApnounceDecrypt.txt","a") as file:  
                    file.write("找到匹配的十六进制串:")
                    file.write(hex(current_value)[2:])
                    file.write("\n")
        break
