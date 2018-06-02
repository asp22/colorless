#ifndef color_me_interface____
#define color_me_interface____

#ifdef __cplusplus
extern "C" {
#endif
	void ColorMe_Initialise();
	void ColorMe_Destory();

	void ColorMe_Configure(const char* path, const char* filename);

	int ColorMe_HaveProcessedLine(int startCharPosition);
	void ColorMe_ProcessLine(int startCharPosition, const char* start, unsigned int length);
	int ColorMe_HasColor(int charPosition);
	unsigned char ColorMe_GetColor(int charPosition);
	const char* ColorMe_GetEscapeString(unsigned char colorIndex);

#ifdef __cplusplus
}
#endif

#endif
