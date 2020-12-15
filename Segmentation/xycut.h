#ifndef XYCUT_H 
#define XYCUT_H

void xycut(Matrix m, int x1, int y1, int x2, int y2, List *L,Matrix m_to_copy);
void x_cut(Matrix m, int x1, int y1, int x2, int y2, List *L,Matrix m_to_copy);
List * segmentation_main(SDL_Surface *img);

#endif
