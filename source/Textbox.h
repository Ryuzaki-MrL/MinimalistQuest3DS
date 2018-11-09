#ifndef TEXTBOX_H
#define TEXTBOX_H

class Renderer;
class Textbox {
  private:
	char buf[256];
	int buflen;
	const char* str;
	int size;
	int current;
	bool skip;
	bool active;

  public:
	Textbox();
	~Textbox();

	void setText(const char* str);
	bool update();
	bool draw(Renderer& render);
};

void textboxCreate(const char* str);
void textboxCreateFormat(const char* str, ...);
Textbox& getTextbox();

#endif /* TEXTBOX_H */