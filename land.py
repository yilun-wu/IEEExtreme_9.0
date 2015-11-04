def PolygonArea(corners):
    n = len(corners) # of corners
    area = 0.0
    for i in range(n):
        j = (i + 1) % n
        area += corners[i][0] * corners[j][1]
        area -= corners[j][0] * corners[i][1]
    area = abs(area) / 2.0
    #print (area)
    return area
# examples
#corners = [(2.0, 1.0), (4.0, 5.0), (7.0, 8.0)]
def check_boundary(x,y,poly):
	n = len(poly)
	for i in range(n):
		start = poly[i]
		end = poly[(i+1)%n]
		if (x == start[0] and y == start[1]) or (x == end[0] and y == end[1]):
			return True
		elif (x - start[0])*(y-end[1]) == (y-start[1])*(x-end[0]) and (x-start[0]) * (x-end[0]) < 0:
			return True
	return False
#print(check_boundary(0.5,2.5,[(0,0), (3,0), (1,2), (3,3), (1,5)]))
		
def point_inside_polygon(x,y,poly):

    n = len(poly)
    inside =False

    p1x,p1y = poly[0]
    for i in range(n+1):
        p2x,p2y = poly[i % n]
        if y > min(p1y,p2y):
            if y <= max(p1y,p2y):
                if x <= max(p1x,p2x):
                    if p1y != p2y:
                        xinters = (y-p1y)*(p2x-p1x)/(p2y-p1y)+p1x
                    if p1x == p2x or x <= xinters:
                        inside = not inside
        p1x,p1y = p2x,p2y
    if inside == False:
	    if check_boundary(x, y, poly):
		    return True
    return inside



def check_tree_inside(poly, tree):
	for i in range(0, len(tree)):
		if point_inside_polygon(tree[i][0], tree[i][1], poly):
			return True
	return False

num_test_cases = int(input())
for i in range(0, num_test_cases):
	poly = []
	tree = []
	num_ver = int(input())
	for i in range(0, num_ver):
		poly.append(tuple(map(int, input().split())))
	#print (poly)
	num_tree = int(input())
	if num_tree == 0:
		print ("%.2f" %PolygonArea(poly))
	else:
		for j in range(0, num_tree):
			tree.append(tuple(map(int, input().split())))

		max_area = 0
		#max_poly = []
		poly = poly+poly
		flag_c = [0]*num_ver
		#flag_n = [0]*num_ver
		for i in range(3, num_ver):
			count = 0
			for j in range(0, num_ver):
				test_poly = poly[j:j+i]
				if not (flag_c[j] or flag_c[(j+1)%num_ver]):
					#print (test_poly)
					if (not check_tree_inside(test_poly, tree)):
						area = PolygonArea(test_poly)
						if area > max_area:
							max_area = area
							max_poly = test_poly
						count += 1
					else:
						flag_c[j] = 1
			
			if count == 0:
				break
		print ("%.2f" %max_area)
		print (max_poly)
#print (tree)
#print (point_inside_polygon(2,,poly))
#print (PolygonArea([(0,0), (3,0), (1,2), (3,3), (1,5)]))
#print(point_inside_polygon(2,-1,[(0,0), (3,0), (1,2), (3,3), (1,5)]))