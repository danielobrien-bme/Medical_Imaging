// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include "BME465_Template.hpp"
#include "sliderFrame.hpp"
// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------
#include "resources/lp.xpm"
#include "resources/hp.xpm"
#include "resources/undo.xpm"

#include "resources/fred.xpm"	//new lab2

#include "resources/Area_Calculation_Icon.xpm"  //new lab6
#include "resources/Binary_Icon.xpm"
#include "resources/Edge_Detect_Icon.xpm"
#include "resources/Maximum_Filter_Icon.xpm"
#include "resources/Minimum_Filter_Icon.xpm"

#include "wx/image.h"


// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(MENU_FILE_OPEN,  MyFrame::OnLoad)
    EVT_MENU(MENU_FILE_QUIT,  MyFrame::OnQuit)
    EVT_MENU(MENU_HELP_ABOUT, MyFrame::OnAbout)
    EVT_MENU(ID_ToGray, MyFrame::OnToGray)

    EVT_PAINT(MyFrame::OnPaint)
    EVT_MENU(MENU_FILTER_LP, MyFrame::OnFilter)
    EVT_MENU(MENU_FILTER_HP, MyFrame::OnFilter)
	
	EVT_MENU(MENU_FILTER_MIN, MyFrame::OnFilter) //new lab2
	EVT_MENU(MENU_FILTER_MED, MyFrame::OnFilter)
	EVT_MENU(MENU_FILTER_MAX, MyFrame::OnFilter)
	
	EVT_MENU(MENU_FILTER_EDGE, MyFrame::OnFilter)	//new lab3
	EVT_MENU(MENU_FILTER_BIN, MyFrame::OnFilter)

	EVT_MENU(MENU_FILTER_2BIN, MyFrame::OnFilter)	//new lab4
	
	
	EVT_LEFT_DOWN(MyFrame::OnLButton)			//new lab5
	EVT_MENU(ID_AreaCalculation, MyFrame::OnAreaCalculation)


	EVT_MENU(MENU_FILTER_UNDO,		MyFrame::OnFilter)


END_EVENT_TABLE()


// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame(_T("BME 465 Template App"),
                                 wxPoint(50, 50), wxSize(800, 600));

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(TRUE);

    // Enable all image handlers - TIFF is among them
    ::wxInitAllImageHandlers();

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------
// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
       : wxFrame(NULL, -1, title, pos, size, style)
{
    // set the frame icon
   // SetIcon(wxICON(mondrian));

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;
    wxMenu *image_processMenu = new wxMenu;
    wxMenu *filterMenu    = new wxMenu;
	
	wxMenu *menuProcess = new wxMenu;		//new lab5


    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    fileMenu->Append(MENU_FILE_OPEN, _T("&Open...\tCtrl-O"), _T("Open a new Image"));
    fileMenu->Append(MENU_FILE_QUIT, _T("E&xit\tAlt-X"), _T("Quit this program"));

    //build filterMenu
    filterMenu->Append(MENU_FILTER_LP, _T("&Lowpass Filter\tAlt-L"), _T("Lowpass Filter"));
    filterMenu->Append(MENU_FILTER_HP, _T("&Highpass Filter\tAlt-H"), _T("Highpass Filter"));
	
	filterMenu->Append(MENU_FILTER_MIN, _T("&Minimum Filter\tAlt-N"), _T("Minimum Filter"));	//new lab2
	filterMenu->Append(MENU_FILTER_MED, _T("&Median Filter\tAlt-D"), _T("Median Filter"));
	filterMenu->Append(MENU_FILTER_MAX, _T("&Maximum Filter\tAlt-X"), _T("Maximum Filter"));

	filterMenu->Append(MENU_FILTER_EDGE, _T("&Edge-Detection Filter\tAlt-E"), _T("Edge-Detection Filter"));	//new lab3
	filterMenu->Append(MENU_FILTER_BIN, _T("&Binary Filter\tAlt-B"), _T("Binary Filter"));
	
	filterMenu->Append(MENU_FILTER_2BIN, _T("&Binarize\tAlt-C"), _T("Binarize"));	//new lab4
	
	//AREA
	filterMenu->Append(ID_AreaCalculation, _T("&Area Calculation\tAlt-A"), _T("Area Calculation"));	//new lab5
	
	
	pointNumber = 99;									//new lab5
		areaborder = new wxPoint[pointNumber];
		areaindex = 0;
	bAreaCalculation = FALSE;
	bleftDown = FALSE;
	
	SetStatusText("Out of Area", 1);					//new lab5
	dist = 9999;
	
	

	image_processMenu->Append(MENU_FILTER,_T("Fil&ters\tAlt-T"),filterMenu, _T("Filter Menu"));

    helpMenu->Append(MENU_HELP_ABOUT, _T("&About...\tF1"), _T("Show about dialog"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, _T("&File"));
    menuBar->Append(image_processMenu, _T("&Image Process"));
    menuBar->Append(helpMenu, _T("&Help"));


	menuProcess->Append(ID_AreaCalculation, _T("&Area Calculation\tAlt-A"), _T("Apply area calculation"));		//new lab5


    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_TOOLBAR
    wxToolBar *MyToolBar = new wxToolBar(this, wxID_ANY);

	MyToolBar->AddTool(MENU_FILTER_LP, _T("Low Pass Filter"),lp_xpm);
	MyToolBar->AddTool(MENU_FILTER_HP, _T("High Pass Filter"),hp_xpm);
	
	MyToolBar->AddTool(MENU_FILTER_MIN, _T("Minumum Filter"),Minimum_Filter_Icon_xpm);	//new lab2
	MyToolBar->AddTool(MENU_FILTER_MED, _T("Median Filter"),fred_xpm);
	MyToolBar->AddTool(MENU_FILTER_MAX, _T("Maximum Filter"),Maximum_Filter_Icon_xpm);

	MyToolBar->AddTool(MENU_FILTER_EDGE, _T("Edge-Detection Filter"),Edge_Detect_Icon_xpm);	//new lab3
	MyToolBar->AddTool(MENU_FILTER_BIN, _T("Binary Filter"), Binary_Icon_xpm);
	
	MyToolBar->AddTool(MENU_FILTER_2BIN, _T("Binarize Filter"),fred_xpm);	//new lab4
	
	
	MyToolBar->AddTool(ID_AreaCalculation, _T("Area Filter"),Area_Calculation_Icon_xpm);	//new lab5
	
	
	MyToolBar->AddTool(MENU_FILTER_UNDO,_T("undo"),undo_xpm);
    SetToolBar(MyToolBar);
    MyToolBar->Realize();


#endif

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(_T("Welcome to BME 465!"));
#endif // wxUSE_STATUSBAR

    pImage = NULL;

	//set the slider stuff to 0
	
	sliderFrame = NULL;	//new lab4
	slider = NULL;
	
	pointNumber = 99;
		areaborder = new wxPoint[pointNumber];
		areaindex = 0;
	bAreaCalculation = FALSE;
	bleftDown = FALSE;

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//slider frame constructor
void MyFrame::makeSlider(int sID, int deflt, int low, int high){	//new lab4
	if (sliderFrame != NULL) {
		delete slider;
		delete sliderFrame;
	}
	sliderID = sID;
		sliderFrame = new MySliderFrame(_T("Slider"), wxPoint(10, 10), wxSize(200,200));
	sliderFrame->MySliderSetParent(this);
		slider = new wxSlider(sliderFrame, SLIDER_ID, deflt, low, high,
								wxDefaultPosition, wxSize(160, wxDefaultCoord),
								wxSL_AUTOTICKS | wxSL_LABELS);
	sliderValue = slider->GetValue();
	sliderFrame->Show(TRUE);
}


void MyFrame::sliderHandler() {			//new lab4
	sliderValue = slider->GetValue();
	switch(sliderID) {
		case BIN_SLIDER: pImage = binarize(masterImage, sliderValue); break;
	}
	Refresh();
}

void MyFrame::sliderWindowCloses(void) {		//new lab4
	sliderFrame = NULL;
	slider = NULL;
}



//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&


void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}


void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( _T("This is the About dialog for BME 465.\n")
                _T("Welcome to %s"), wxVERSION_STRING);

    wxMessageBox(msg, _T("About BME 465"), wxOK | wxICON_INFORMATION,this);
}
void MyFrame::OnLoad(wxCommandEvent& WXUNUSED(event))
{

    wxString fileName;
    wxString NameofFile;

    wxFileDialog fileDialog(NULL, _T("Choose a file to load ..."), wxEmptyString, wxEmptyString, _T("*.tiff;*.tif;*.gif;*.jpg"), wxFD_OPEN);
    if( fileDialog.ShowModal() == wxID_OK )

    {
        fileName = fileDialog.GetPath();
        NameofFile = fileDialog.GetFilename();
        //Update the view of main frame
        if( pImage != NULL ) delete pImage;
        pImage = new wxImage(fileName);

        if( pImage == NULL )
            wxMessageBox("File Failed to Open!", _T("Error"), wxOK | wxICON_INFORMATION, this);
        else
        {
		masterImage = pImage;
		SetTitle(fileName);
		Refresh();
        }
    }
    fileDialog.Destroy();
}
void MyFrame::OnToGray(wxCommandEvent& event)
{
    if( pImage == NULL )
    {
        wxMessageBox("Image is not loaded yet!", _T("Error"), wxOK | wxICON_INFORMATION, this);
        return;
    }

    int* buffer;
    buffer = new int[ pImage->GetWidth()*pImage->GetHeight() ];
    wxImage2grayBuffer(pImage,buffer);

    wxImage* temp;
    temp = grayBuffer2wxImage(buffer, pImage->GetWidth(),pImage->GetHeight() );
    delete pImage;
    pImage = temp;
    delete buffer;

    Refresh();
    return;
}

void MyFrame::OnPaint(wxPaintEvent& event)
{

    wxPaintDC dc(this);
	int spacingW = 0;
	int spacingH = 0;

    if( pImage == NULL )
    {
        event.Skip();
    }
    else
    {
        wxBitmap tempBitmap(*pImage);
        SetClientSize( tempBitmap.GetWidth()*1.25, tempBitmap.GetHeight()*1.25+10 );
		spacingW = ceilf((tempBitmap.GetWidth()*1.25 - tempBitmap.GetWidth())/2);
		spacingH = ceilf((tempBitmap.GetHeight()*1.25 - tempBitmap.GetHeight())/2);
        dc.DrawBitmap(tempBitmap,(int)spacingW,(int)spacingH+25, TRUE);
    }

	//AREA
	if(bleftDown)
		pointNumber = areaindex;
	else if(dist>=0 && dist<5){
		pointNumber = areaindex;
		int idist = (int)(floor(dist));
		SetStatusText( wxString::Format("The distance is %d", idist),0);
	}
	
	//AREA
	if((areaindex >0) && (areaindex <=pointNumber))	//If in area calculation mode
	{
	//wxPoint pt(event.GetLogicalPosition(dc));
		wxPoint* acborder_pt=new wxPoint[pointNumber];	//the points coord to be drawn
		for(int i = 0; i< areaindex; i++)
		{
			acborder_pt[i].x = dc.LogicalToDeviceX(areaborder[i].x);
			acborder_pt[i].y = dc.LogicalToDeviceY(areaborder[i].y);
		}
		dc.SetPen(*wxRED_PEN);
		//dc.Drawline(pt.x, pt.y, acborder_pt[areaindex].x, acborder_pt[areaindex].y);
		dc.DrawLines(areaindex, acborder_pt);
		if(areaindex == pointNumber)
		{
			dc.DrawLine(acborder_pt[pointNumber - 1].x, acborder_pt[pointNumber - 1].y, 
			acborder_pt[0].x, acborder_pt[0].y);
			
			float farea = 0;
			int area;
			int minborder = 9999;
				for (int i = 0; i < pointNumber; i++){
					if (areaborder[i].x < minborder)
						minborder = areaborder[i].x;
				}
				
				for (int j = 0; j<pointNumber-1; j++){
					farea = ((float)((areaborder[j].x - minborder) 
					+ (areaborder[j+1].x - minborder)) / 2.0)
					* (areaborder[j+1].y - areaborder[j].y) + farea;
				
				}
				farea = ((float)((areaborder[pointNumber-1].x - minborder) 
				+ (areaborder[0].x - minborder)) / 2.0)
				* (areaborder[0].y - areaborder[pointNumber-1].y) + farea;
				if(farea<0) farea = -farea;
				area = (int)farea;
				SetStatusText(wxString::Format("The area is %d", area),1);
				
				areaindex = 0;
				bleftDown = !bleftDown;
				bRed = FALSE;
				pointNumber = 99;
				dist = 9999;
		}
			delete acborder_pt;
			
	}
	




    return;
}

void MyFrame::OnAreaCalculation( wxCommandEvent& WXUNUSED(event))	//new lab5
{
	bAreaCalculation = !bAreaCalculation;
	if(bAreaCalculation)
		SetStatusText("In Area",1);
	else
		SetStatusText("Out of Area", 1);
	Refresh();
	return;
}

void MyFrame::OnLButton(wxMouseEvent& event)		//new lab5
{
	wxClientDC dc(this);
	wxPoint area_pt(event.GetLogicalPosition(dc));
	
	SetStatusText(wxString::Format("(%d,%d)", area_pt.x, area_pt.y),0);
	
	if(!bAreaCalculation)
		return;
		
	if(pImage == NULL)
	{
		wxMessageBox("Image is not loaded yet!", _T("Error"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	areaborder[areaindex].x = area_pt.x;
	areaborder[areaindex].y = area_pt.y;
	if (areaindex>0)
		dist = sqrt((pow((double)(area_pt.x - areaborder[0].x),2)+pow((double)(area_pt.y 
		 - areaborder[0].y),2)));
	SetStatusText(wxString::Format("(%d,%d,%d)", area_pt.x, area_pt.y,(int)dist),1);
	areaindex++;
	Refresh();

}



void MyFrame::OnFilter( wxCommandEvent& event )
{


        if( pImage == NULL )
        {
            wxMessageBox("Image is not loaded yet!", _T("Error"), wxOK | wxICON_INFORMATION,this);
        }
        else
        {
            wxImage *Filtered = NULL;

            switch(event.GetId())
            {
				case MENU_FILTER_LP: Filtered = LowPass(pImage); break;
				case MENU_FILTER_HP: Filtered = HighPass(pImage); break;
				case MENU_FILTER_UNDO: Filtered = copy(masterImage); break;
				
				case MENU_FILTER_MIN: Filtered = Nonlinear(pImage, MENU_FILTER_MIN); break;	//new lab2
				case MENU_FILTER_MED: Filtered = Nonlinear(pImage, MENU_FILTER_MED); break;
				case MENU_FILTER_MAX: Filtered = Nonlinear(pImage, MENU_FILTER_MAX); break;

				case MENU_FILTER_EDGE: Filtered = Edge(pImage); break;		//new lab3
				case MENU_FILTER_BIN: Filtered = Binary(pImage); break;
				
				case MENU_FILTER_2BIN: { makeSlider(BIN_SLIDER, 128,0,255);	//new lab4
					Filtered = binarize(masterImage, sliderValue);}
				break;
			}

            pImage = Filtered;
         }
    Refresh();
    return;
}
