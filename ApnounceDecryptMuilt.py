import hashlib
from multiprocessing import Pool, cpu_count
cnt = 0
def calculate_sha1_range(chunk):
    results = []
    for current_value in chunk:
        hex_string = hex(current_value)[2:]
        hash_object = hashlib.sha1(bytes.fromhex(hex_string))
        current_sha1 = hash_object.hexdigest()
        results.append((current_value, current_sha1))
    return results

def find_matching_hex(target_sha1, start_value, end_value):
    global cnt
    # 获取CPU核心数
    num_cores = cpu_count()
    # 构造参数列表
    values_to_check = range(start_value, end_value + 1)

    # 将values_to_check分成多个小块
    chunk_size = len(values_to_check) // num_cores
    chunks = [values_to_check[i:i + chunk_size] for i in range(0, len(values_to_check), chunk_size)]

    # 创建进程池
    with Pool(num_cores) as pool:
        # 使用map函数并行计算SHA-1值
        results = pool.map(calculate_sha1_range, chunks)
        #print("hello,world!")
        # 将结果合并
        flattened_results = [item for sublist in results for item in sublist]

        # 遍历结果查找匹配
        for current_value, current_sha1 in flattened_results:
            cnt = cnt+1
            #print(target_sha1)
            if cnt >= 10000000:
                #print("hello,world!")
                #print(hex(current_value)[2:], current_sha1)
                with open("ApnounceDecryptMuilt.txt","a") as file: 
                    file.write(hex(current_value)[2:])
                    file.write(" ")
                    file.write(current_sha1)
                    file.write("\n")
                    file.close()
                cnt = 0
            if current_sha1 == target_sha1 :
                with open("ApnounceDecryptMuilt.txt","a") as file:  
                    file.write("找到匹配的十六进制串:")
                    file.write(hex(current_value)[2:])
                    file.write("\n")
                break

if __name__ == "__main__":

    target_sha1 = "abc21d3f9d5d98ecae9f3d514475290171f2bbed"
    start_value = 0x31111114a2b0d2dc
    #print(target_sha1)
    #3111111161ae942d
    end_value = 0x3111ffffffffffff
    i = 5000000
    while start_value + i <= end_value:
        find_matching_hex(target_sha1, start_value, start_value+i)
        start_value = start_value + i
    else:
        find_matching_hex(target_sha1, start_value, end_value)
