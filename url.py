b62tab = [str(a) for a in range(10)]+[chr(a + ord('a')) for a in range(26)] + [chr(a + ord('A')) for a in range(26)]
def base62(a):
	if a == 0:
		return "0"
	ret=[]
	while a != 0:
		ret.append(b62tab[a % 62])
		a = a // 62
	return "".join(ret[::-1])
def generate_url(b_url, t_url):
	if len(t_url) > len(b_url):
		#repeat base url
		b_url += b_url * ((len(t_url) - len(b_url)) // len(b_url)) + b_url[0:(len(t_url) - len(b_url)) % len(b_url)]
	elif len(t_url) < len(b_url):
		#repeat target url
		t_url += t_url * ((len(b_url) - len(t_url)) // len(t_url)) + t_url[0:(len(b_url) - len(t_url)) % len(t_url)]
	bb = b_url.encode('utf-8')
	tb = t_url.encode('utf-8')
	nb = []
	for i in range(0,len(bb)):
		nb.append(bb[i]^tb[i])
		#bb[i] = bb[i]^tb[i]
		#print("{:x}".format(nb[i]))
	last_8 = nb[len(nb) - 8:]
	#print (last_8, sep ='')
	result_int = 0
	for i in range(0,8):
		result_int += last_8[i] * 256**(7-i)
	print(b_url_unmodified + "/" + base62(result_int))
	

b_url = input()
b_url_unmodified = b_url
num_test_cases = int(input())
for i in range(num_test_cases):
	generate_url(b_url, input())