typedef struct{
	size_t x;
	size_t y;
	size_t z;
} uivec3;

typedef struct{
	size_t x;
	size_t y;
} uivec2;

#define EMPTY (- 1)

float plane_distance(vec3 v1, vec3 v2) {
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	return sqrt(dx*dx + dy*dy);
}

size_t find_closest(vec3 * vertices, size_t n, size actual_point){
	size_t acc = 0;
	float dist = FLT_MAX;
	for(int i = 0; i < n; i++){
		if (i != actual_point)
			if (plane_distance(vertices[actual_point], vertices[i]) < dist)
				acc = i;
	}
	return acc;
}

bool onSegment(vec3 a, vec3 q, vec3 r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
       return true;

    return false;
}

int orientation(vec3 p, vec3 q, vec3 r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

bool is_intersected(vec3 p1, vec3 q1, vec3 p2, vec3 q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return true;

    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

bool generate_segment(vec3 * vertices, size_t n, uivec2 * segments, size_t * seg_bound, uivec2 wanted_seg){
		//Verify that no segments are crossed
		for(int i = 0; i < *seg_bound; i++)
			if (is_intersected(vertices[wanted_seg.x], vertices[wanted_seg.y], vertices[segments[i].x], vertices[segments[i].y])
				return false;
		segments[(*seg_bound)++] = wanted_seg;
}


int generate_faces(vec3 * vertices, size_t n, uivec2 * segments, size_t seg_bound, uivec3 * faces, size_t *face_bound, uivec2 last_seg){
	uivec2 seg2, seg3;
	int acc;
	for (int i = 0; i < seg_bound; i++) {
		seg2 = segments[i];
		for (int k = i; k < seg_bound; k++) {
			seg3 = segments[k];
			if (seg2.y == seg3.x){
				if (lastseg.y == seg2.x && seg3.y == last_seg.x) {
					acc++;
					faces[*face_bound+acc].x = last_seg.x;
					faces[*face_bound+acc].y = seg2.x;
					faces[*face_bound+acc].z = seg3.x;
				}
			} else if (seg2.x == seg3.y) {
				if (lastseg.x == seg2.y && seg3.x == last_seg.y) {
					acc++;
					faces[*face_bound+acc].x = last_seg.x;
					faces[*face_bound+acc].y = seg3.x;
					faces[*face_bound+acc].z = seg2.x;
				}
			}
		}
	}

	return acc;
}

void generate_plane_faces(vec3 * vertices, size_t n) {
	//Instanciating all tools
	size_t seg_bound = 3*n-7;
	size_t active_seg_bound = 4;
	size_t face_bound = 2*n - 6;
	size_t active_face_bound = 0;
	uivec2 segments [seg_bound];
	uivec3 faces [face_bound];
	bool black_list[number][number];
	memset(segments, EMPTY, seg_bound * sizeof(uivec2));
	memset(faces, EMPTY, face_bound * sizeof(uivec3));

	//Creating the base segments : 1-2, 2-3, 3-4, 4-1
	segments[0].x = 0; segments[0].y = 1;
	segments[1].x = 1; segments[1].y = 2;
	segments[2].x = 2; segments[2].y = 3;
	segments[3].x = 3; segments[3].y = 0

	size_t actual_point = 0;

	//FIND THE CLOSEST POINT :
	size_t next_point = find_closest(vertices, n, actual_point);
	//GENERATE A SEGMENT :
	if (generate_segment(vertices, n)){
		int new_faces = generate_face(vertices, n);
		if (new_faces) {

		} else {

		}
	} else {

	}

}
