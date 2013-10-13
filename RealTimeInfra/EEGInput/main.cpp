#include <iostream>
#include <fstream>
#include <conio.h>
#include <sstream>
#include <windows.h>
#include <map>
#include <time.h>

#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"
//#include "zed_form.h"

#include "stdafx.h"

//using namespace System;
//using namespace System::Collections;
//using namespace System::Data;
//using namespace System::Drawing;
//using namespace System::Drawing::Imaging;
////using namespace System::Windows::Forms;
//using namespace ZedGraph;
// 
//void CreateGraph(String^ fileName, double XArray[38], double YArray[38]);


#pragma comment(lib, "../lib/edk.lib")
#define NUM_OF_LINES 1000
#define CH_PER_LINE 1
#define BUFFER_SIZE (NUM_OF_LINES*CH_PER_LINE)
#define BUFFER2SEND_SIZE 400
#define CALL_CLASIFY_RATE 400

#define N_DATA_POINTS 400
#define N_CHANNELS 14

int ClassifierV1(double data[]) ;
int ClassifierV3(double data[]); 

int ClassifierBoth(double data[]) {
	return ClassifierV1(data) || ClassifierV3(data);
}

#define Classifier ClassifierBoth

EE_DataChannel_t targetChannelList[] = {
		ED_COUNTER,
		ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
		ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
		ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
		ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
	};

const char header[] = "COUNTER,AF3,F7,F3, FC5, T7, P7, O1, O2,P8" 
                      ", T8, FC6, F4,F8, AF4,GYROX, GYROY, TIMESTAMP, "   
                      "FUNC_ID, FUNC_VALUE, MARKER, SYNC_SIGNAL,";

unsigned long global_counter = 0;
const int nch = 15; //sizeof(targetChannelList)/sizeof(EE_DataChannel_t);
double buffer[BUFFER_SIZE*nch];
double buffer2send[BUFFER2SEND_SIZE*nch];
int index_in_buffer = 0;
SYSTEMTIME st;

DWORD WINAPI PlaySoundThread(LPVOID lpParameter)
{
	PlaySound(TEXT("hiyha-beep.wav"), NULL, SND_FILENAME);
	return 0;
}

void PlaySoundNonBlock(int cnt)
{
	static int last_cnt=0;
	std::cout << std::endl << "PULL YOUR HAND NOW!" << std::endl << std::endl;										
	if (cnt - last_cnt<1000) return;
	CreateThread(NULL,0,PlaySoundThread,0,0,NULL);
	last_cnt = cnt;
}

int printline(double data[BUFFER2SEND_SIZE*nch])
{
	
	for (int t=(BUFFER2SEND_SIZE-1)*nch ; t<BUFFER2SEND_SIZE*nch-10; t++) {
	std::cout << data[t] << ",";
	
	}
	//std::ofstream ofs(argv[1],std::ios::trunc);
	//ofs << header << std::endl;
	//ofs << buffer2send << std::endl;

	/*for (int i=0; i<BUFFER_SIZE; i++) {
		ofs << global_counter << ":";
		for (int j=0; j<nch; j++) {
			ofs << data[i][j] << ",";
		}
		ofs << std::endl;
	}*/
	//ofs.close();
	//std::cout << "\n";*/
	//if (global_counter%1000 == 0)
		//PlaySoundNonBlock();
	return 0;
	
}
//[STAThreadAttribute]
//int main(array<System::String ^> ^args) { 
//	Application::EnableVisualStyles();
//	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	//Application::Run(gcnew zed_form);
int main(int argc, char** argv) {

	EmoEngineEventHandle eEvent			= EE_EmoEngineEventCreate();
	EmoStateHandle eState				= EE_EmoStateCreate();
	unsigned int userID					= 0;
	const unsigned short composerPort	= 1726;
	float secs							= 1;
	unsigned int datarate				= 0;
	bool readytocollect					= false;
	int option							= 0;
	int state							= 0;
	HWND MyWinHandle;
	
	std::string input;

	//MessageBox(HWND_DESKTOP,"PULL YOUR HANDS NOW!"," ",(MB_OK|MB_TOPMOST));
	//MyWinHandle = CreateWindow(NULL,NULL,WS_BORDER,0,0,1000,1000,NULL,NULL,NULL,NULL);
	//ShowWindow(MyWinHandle ,SW_MAXIMIZE);
//	String^ fileName;
//	fileName = "C:\\TestGraph.png";
	//CreateGraph(fileName);

	
	try {
		
		if (argc != 2) {
			throw std::exception("Please supply the log file name.\nUsage: EEGLogger [log_file_name].");
		}
		std::cout << "===================================================================" << std::endl;
		std::cout << "Example to show how to log EEG Data from EmoEngine/EmoComposer."	   << std::endl;
		std::cout << "===================================================================" << std::endl;
		std::cout << "Press '1' to start and connect to the EmoEngine                    " << std::endl;
		std::cout << "Press '2' to connect to the EmoComposer                            " << std::endl;
		std::cout << ">> ";

		std::getline(std::cin, input, '\n');
		option = atoi(input.c_str());  
		//option = 1; // means start and connect to the EmoEngine
		switch (option) {
			case 1:
			{
				if (EE_EngineConnect() != EDK_OK) {
					throw std::exception("Emotiv Engine start up failed.");
				}
				break;
			}
			case 2:
			{
				std::cout << "Target IP of EmoComposer? [127.0.0.1] ";
				std::getline(std::cin, input, '\n');

				if (input.empty()) {
					input = std::string("127.0.0.1");
				}

				if (EE_EngineRemoteConnect(input.c_str(), composerPort) != EDK_OK) {
					std::string errMsg = "Cannot connect to EmoComposer on [" + input + "]";
					throw std::exception(errMsg.c_str());
				}
				break;
			}
			default:
				throw std::exception("Invalid option...");
				break;
		}
		
		
		std::cout << "Start receiving EEG Data! Press any key to stop logging...\n" << std::endl;

		std::ofstream ofs(argv[1],std::ios::trunc);
		ofs << header << std::endl;
    	
		
		DataHandle hData = EE_DataCreate();
		EE_DataSetBufferSizeInSec(1);

		std::cout << "Buffer size in secs:" << secs << std::endl;

		while (!_kbhit()) {
			
			state = EE_EngineGetNextEvent(eEvent);

			if (state == EDK_OK) {

				EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
				EE_EmoEngineEventGetUserId(eEvent, &userID);

				// Log the EmoState if it has been updated
				if (eventType == EE_UserAdded) {
					std::cout << "User added";
					EE_DataAcquisitionEnable(userID,true);
					readytocollect = true;
				}
			}

			if (readytocollect) {
						
						EE_DataUpdateHandle(0, hData);

						unsigned int nSamplesTaken=0;
						EE_DataGetNumberOfSample(hData,&nSamplesTaken);
						if (global_counter >= BUFFER_SIZE)
						{
							//memmove(buffer, buffer+(nSamplesTaken*sizeof(double)*nch), sizeof(double)*nch*(BUFFER_SIZE - nSamplesTaken));
						}
		
						
						
						// current sample of  nSamplesTaken time points
						if (nSamplesTaken != 0) {							
							double* data = new double[nSamplesTaken];
							double* all_ch_data_one_sample = new double[sizeof(targetChannelList)];
							int classify_result = 0;
							std::cout << "Updated " << nSamplesTaken << std::endl;
							GetSystemTime(&st);
							// go over all time points in sample
							for (int sampleIdx=0 ; sampleIdx<(int)nSamplesTaken ; ++ sampleIdx) {
								// go over all channels
								global_counter++;
								for (int i = 0 ; i < nch ; i++) {

									EE_DataGet(hData, targetChannelList[i], data, nSamplesTaken);
									//ofs << data[sampleIdx] << ","; // TODO
									all_ch_data_one_sample[i] = data[sampleIdx];	
									
									index_in_buffer = global_counter%BUFFER_SIZE;
									buffer[index_in_buffer*nch+i] = data[sampleIdx];
									
								}
								//printf("%d\n", global_counter);
								for (int t=0 ; t<nch; t++) {
									ofs << all_ch_data_one_sample[t] << ",";
								}
								ofs << st.wHour << ","<< st.wMinute <<","<< st.wSecond<<","<<st.wMilliseconds << "," ;
								if (sampleIdx < ((int)nSamplesTaken-1))
									ofs << -1 << "," << std::endl;

								
							}

							if (global_counter >=BUFFER2SEND_SIZE) {
								if (index_in_buffer >= BUFFER2SEND_SIZE) {
									//printline(buffer+(index_in_buffer-BUFFER2SEND_SIZE)*nch);
									Classifier(buffer+(index_in_buffer-BUFFER2SEND_SIZE)*nch); 
									if (classify_result==1) {
										PlaySoundNonBlock(global_counter);//, BUFFER2SEND_SIZE, argv);
										//MessageBox(HWND_DESKTOP," ","PULL YOUR HAND NOW!",(MB_OK|MB_TOPMOST));
										
									}
								} else {
									memcpy(buffer2send,buffer+(BUFFER_SIZE-BUFFER2SEND_SIZE+index_in_buffer)*nch,
										(BUFFER2SEND_SIZE-index_in_buffer)*nch*sizeof(buffer2send[0]));
										
									memcpy(buffer2send+(BUFFER2SEND_SIZE-index_in_buffer)*nch,
										buffer,
										index_in_buffer*nch*sizeof(buffer2send[0]));
									//printline(buffer2send);
									classify_result = Classifier(buffer2send);
									if (classify_result ==1) {
										PlaySoundNonBlock(global_counter);// BUFFER2SEND_SIZE, argv);
										//MessageBox(HWND_DESKTOP," ","PULL YOUR HAND NOW!",(MB_OK|MB_TOPMOST));
									}
									ofs << classify_result << "," << std::endl;
								}
								//return 0;
							}
							
							delete[] data;
						}

			}

			//Sleep(100);
		}

		ofs.close();
		EE_DataFree(hData);

	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::cout << "Press any key to exit..." << std::endl;
		getchar();
	}

	EE_EngineDisconnect();
	EE_EmoStateFree(eState);
	EE_EmoEngineEventFree(eEvent);

	return 0;

	
}

//void CreateGraph(String^ fileName, double XArray[38], double YArray[38])
//{
//    // Create a GraphPane is really all you have to do
//    // when using ZedGraph in a console application
//    RectangleF rect = RectangleF( 0, 0, 640, 480 );
//    GraphPane ^myPane = gcnew GraphPane( rect, "Überschrift", "X Titel", "Y Titel" );
// 
//    // Set the titles and axis labels
//    myPane->Title->Text = L"EEG From Emotiv";
//    myPane->XAxis->Title->Text = L" ";
//    myPane->YAxis->Title->Text = L" ";
//    myPane->Y2Axis->Title->Text = L" ";
// 
//    // Make up some data points based on the Sine function
//    PointPairList ^list = gcnew PointPairList();
//    PointPairList ^list2 = gcnew PointPairList();
//    for ( int i = 0; i < 39; i++ )
//    {
//        double x = XArray[i];
//        double y = YArray[i];
//        double y2 = y * 13.5;
//        list->Add( x, y );
//        list2->Add( x, y2 );
//    }
// 
//    // Generate a red curve with diamond symbols, and "Alpha" in the legend
//    LineItem ^myCurve = myPane->AddCurve( L" ",
//        list, Color::Red, SymbolType::Diamond );
//    // Fill the symbols with white
//    myCurve->Symbol->Fill = gcnew Fill( Color::White );
// 
//    // Generate a blue curve with circle symbols, and "Beta" in the legend
//    myCurve = myPane->AddCurve( L" ",
//        list2, Color::Blue, SymbolType::Circle );
//    // Fill the symbols with white
//    myCurve->Symbol->Fill = gcnew Fill( Color::White );
//    // Associate this curve with the Y2 axis
//    myCurve->IsY2Axis = true;
// 
//    // Show the x axis grid
//    myPane->XAxis->MajorGrid->IsVisible = true;
// 
//    // Make the Y axis scale red
//    myPane->YAxis->Scale->FontSpec->FontColor = Color::Red;
//    myPane->YAxis->Title->FontSpec->FontColor = Color::Red;
//    // turn off the opposite tics so the Y tics don't show up on the Y2 axis
//    myPane->YAxis->MajorTic->IsOpposite = false;
//    myPane->YAxis->MinorTic->IsOpposite = false;
//    // Don't display the Y zero line
//    myPane->YAxis->MajorGrid->IsZeroLine = false;
//    // Align the Y axis labels so they are flush to the axis
//    myPane->YAxis->Scale->Align = AlignP::Inside;
//    // Manually set the axis range
//    myPane->YAxis->Scale->Min = -30;
//    myPane->YAxis->Scale->Max = 30;
// 
//    // Enable the Y2 axis display
//    myPane->Y2Axis->IsVisible = true;
//    // Make the Y2 axis scale blue
//    myPane->Y2Axis->Scale->FontSpec->FontColor = Color::Blue;
//    myPane->Y2Axis->Title->FontSpec->FontColor = Color::Blue;
//    // turn off the opposite tics so the Y2 tics don't show up on the Y axis
//    myPane->Y2Axis->MajorTic->IsOpposite = false;
//    myPane->Y2Axis->MinorTic->IsOpposite = false;
//    // Display the Y2 axis grid lines
//    myPane->Y2Axis->MajorGrid->IsVisible = true;
//    // Align the Y2 axis labels so they are flush to the axis
//    myPane->Y2Axis->Scale->Align = AlignP::Inside;
// 
//    // Fill the axis background with a gradient
//    myPane->Chart->Fill = gcnew Fill( Color::White, Color::LightGray, 45.0f );
// 
//    Bitmap^ bm = gcnew Bitmap( 1, 1 );
//    Graphics^ g = Graphics::FromImage( bm );
//    myPane->AxisChange( g );
//    // Retrieve the image
//    Image^ img = myPane->GetImage();
//    // Save image as PNG
//    img->Save(fileName, ImageFormat::Png);//, ImageFormat::Bmp);
//}


