#include <Application.h>
#include <Window.h>
#include <View.h>
#include <TextControl.h>
#include <Button.h>
#include <RadioButton.h>
#include <GroupLayout.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <GroupView.h>

enum {
    M_CONVERT_PRESSED = 'cnvt',
    M_DIRECTION_CHANGED = 'dirg'
};

class MainWindow : public BWindow {
public:
    MainWindow(void)
        : BWindow(BRect(100, 100, 400, 300), "Temperature Converter", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
    {
        // Create input field
        fInputField = new BTextControl("Input:", "", NULL);
        
        // Create output field (read-only)
        fOutputField = new BTextControl("Result:", "", NULL);
        fOutputField->SetEnabled(false);
        
        // Create convert button
        fConvertButton = new BButton("Convert", new BMessage(M_CONVERT_PRESSED));
        
        // Create radio buttons for direction
        fCtoFRadio = new BRadioButton("Celsius to Fahrenheit", new BMessage(M_DIRECTION_CHANGED));
        fFtoCRadio = new BRadioButton("Fahrenheit to Celsius", new BMessage(M_DIRECTION_CHANGED));
        fCtoFRadio->SetValue(B_CONTROL_ON); // Default selection
        
        // Create a group for radio buttons
        BGroupView* radioGroup = new BGroupView(B_VERTICAL);
        radioGroup->GroupLayout()->AddView(fCtoFRadio);
        radioGroup->GroupLayout()->AddView(fFtoCRadio);

        BLayoutBuilder::Group<>(this, B_VERTICAL)
            .SetInsets(10, 10, 10, 10)
            .AddGrid()
                .Add(fInputField->CreateLabelLayoutItem(), 0, 0)
                .Add(fInputField->CreateTextViewLayoutItem(), 1, 0)
                .Add(fOutputField->CreateLabelLayoutItem(), 0, 1)
                .Add(fOutputField->CreateTextViewLayoutItem(), 1, 1)
            .End()
            .Add(radioGroup)
            .AddGroup(B_HORIZONTAL)
                .AddGlue()
                .Add(fConvertButton)
            .End();
            
        UpdateLabels();
    }

    void MessageReceived(BMessage *msg) {
        switch (msg->what) {
            case M_CONVERT_PRESSED:
                ConvertTemperature();
                break;
            case M_DIRECTION_CHANGED:
                UpdateLabels();
                break;
            default:
                BWindow::MessageReceived(msg);
        }
    }

    bool QuitRequested() {
        be_app->PostMessage(B_QUIT_REQUESTED);
        return true;
    }

private:
    void UpdateLabels() {
        if (fCtoFRadio->Value() == B_CONTROL_ON) {
            fInputField->SetLabel("Celsius:");
            fOutputField->SetLabel("Fahrenheit:");
        } else {
            fInputField->SetLabel("Fahrenheit:");
            fOutputField->SetLabel("Celsius:");
        }
    }

    void ConvertTemperature() {
        const char* inputText = fInputField->Text();
        if (strlen(inputText) > 0) {
            float input = atof(inputText);
            float result;
            
            if (fCtoFRadio->Value() == B_CONTROL_ON) {
                // Celsius to Fahrenheit
                result = (input * 9.0f / 5.0f) + 32.0f;
            } else {
                // Fahrenheit to Celsius
                result = (input - 32.0f) * 5.0f / 9.0f;
            }
            
            BString output;
            output.SetToFormat("%.1f", result);
            fOutputField->SetText(output.String());
        }
    }

    BTextControl* fInputField;
    BTextControl* fOutputField;
    BButton* fConvertButton;
    BRadioButton* fCtoFRadio;
    BRadioButton* fFtoCRadio;
};

// Application class and main function remain the same as above
class Application : public BApplication {
public:
    Application(void)
        : BApplication("application/x-vnd.TempConverter")
    {
        MainWindow *window = new MainWindow();
        window->Show();
    }
};

int main(void) {
    Application app;
    app.Run();
    return 0;
}

//g++ -o TemperatureConverter main.cpp -lbe -lroot
