#ifndef UIPANEL_H
#define UIPANEL_H

class UiPanel
{
public:
	UiPanel(const char* name);
	virtual ~UiPanel();

	virtual void Draw(const char* title);
	
	void Active();

	bool IsActive()const;

	virtual void Draw() = 0;

public:

	bool active = false;

protected:
	const char* name;

};

#endif // !UIPANEL_H
