#include "SDL.h"
#include "SDL_gpu.h"
#include "SDL_gpuShapes.h"
#include <math.h>

void printRenderers(void)
{
	const char* renderers[GPU_GetNumRegisteredRenderers()];
	GPU_GetRegisteredRendererList(renderers);
	
	printf("Available renderers:\n");
	int i;
	for(i = 0; i < GPU_GetNumRegisteredRenderers(); i++)
	{
		printf("%d) %s\n", i+1, renderers[i]);
	}
}

int main(int argc, char* argv[])
{
	printRenderers();
	
	GPU_Target* screen = GPU_Init(NULL, 800, 600, 0);
	if(screen == NULL)
		return -1;
	
	printf("Using renderer: %s\n", GPU_GetCurrentRendererID());
	
	
	Uint32 startTime = SDL_GetTicks();
	long frameCount = 0;
	
	
	GPU_Image* img = GPU_LoadImage("data/test4.bmp");
	
	int i = 0;
	
	int numPolys = 4;
	int pn[numPolys];
	float* pv[numPolys];
	for(i = 0; i < numPolys; i++)
	{
		pn[i] = rand()%8;
		pv[i] = (float*)malloc(2*pn[i]*sizeof(float));
		int j;
		for(j = 0; j < pn[i]*2; j+=2)
		{
			pv[i][j] = rand()%screen->w;
			pv[i][j+1] = rand()%screen->h;
		}
	}
	
	Uint8 done = 0;
	SDL_Event event;
	while(!done)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				done = 1;
			else if(event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym == SDLK_ESCAPE)
					done = 1;
			}
		}
		
		GPU_Clear(screen);
		
		for(i = 0; i < numPolys; i++)
		{
			GPU_PolygonBlit(img, NULL, screen, pn[i], pv[i], pv[i][0], pv[i][1], 0.0f, 1.0f, 1.0f);
		}
		
		
		GPU_Flip();
		
		frameCount++;
		if(frameCount%500 == 0)
			printf("Average FPS: %.2f\n", 1000.0f*frameCount/(SDL_GetTicks() - startTime));
	}
	
	printf("Average FPS: %.2f\n", 1000.0f*frameCount/(SDL_GetTicks() - startTime));
	
	for(i = 0; i < numPolys; i++)
	{
		free(pv[i]);
	}
	
	GPU_Quit();
	
	return 0;
}


