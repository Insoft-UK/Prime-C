//#define MY_DEFINE "Copyright (c) 2023 Insoft. All rights reserved."

l = 10 * 320 + 10;
l += 10 * 320 + 10;

Int START()
{
    List<Int64> blobA = {0x03E07C00007F7FE0,0x007F7FE003E07C00,0x7FFF00007FFF0000,0x00007FFF00007FFF};
    
    RECT_P(G0, 0, 0, 320, 240, RGB(0, 0, 0, 0));
    DIMGROB_P(G2, 4, 4, blobA);
    BLIT_P(G0, 0, 0, 200, 200, G2, 0, 0, 4, 4);
    
    DIMGROB_P(G3, 1, 1, RGB(255, 255, 0));
    BLIT_P(G0, 300, 0, 320, 200, G3, 0, 0, 4, 4);
    
    Int l;
    Int index = 0;
    List<Int64> grayscale(__LIST_LIMIT);
    for (l = 0; l < 256; l += 2) {
        grayscale[index] = (Int64)RGB(l + 1, l + 1, l + 1);
        grayscale[index] = BITSL(grayscale[index], 32);
        grayscale[(Int)index] = BITOR(grayscale[index], RGB(l, l, l));
        index += 1;
    }
    
    DIMGROB_P(G4, 256, 1, grayscale);
    BLIT_P(G0, 0, 0, 256, 256, G4, 0, 0, 256, 1);
    
    while(true) {
        SLEEP;
    }
    
    switch (l) {
        case 1:
            break;
        case 2:
            switch (l) {
                case 1:
                    break;
                default:
            }
            break;
        case 3:
            break;
        default:
    }
    
    l = 10 * 320 + 10;
    
    return 0;
}



