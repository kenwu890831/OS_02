# include<fstream>
# include<iostream>
# include<vector>
#include<algorithm>
#include <queue>
using namespace std;

struct Data{ // 取消清單
    char name ;
    int ID;
    int CPUBurst ;
    int ArrivalTime ;
    int Priority ;
    int runTime = 0;
};

struct resultData{ // 取消清單
    int ID ;
    int FCFS ;
    int RR ;
    int SRTF ;
    int PPRR ;
    int HRRN ;
};

struct processRunList{ // 取消清單
    char runList ;
};

   bool sortArrival( const Data &a, const Data &b ) {
        if (a.ArrivalTime > b.ArrivalTime)
            return true;
        else if (a.ArrivalTime == b.ArrivalTime && a.ID > b.ID)
            return true;
        else
            return false;
    }

       bool sortShortTime( const Data &a, const Data &b ) {
        int aTime = a.CPUBurst-a.runTime ;
        int bTime = b.CPUBurst-b.runTime ;
        if ( aTime > bTime)
            return true ;
        else if (aTime == bTime &&a.ArrivalTime > b.ArrivalTime)
            return true;
        else if (a.ArrivalTime == b.ArrivalTime && a.ID > b.ID)
            return true;
        else
            return false;
    }

   bool sortID( const resultData &a, const resultData &b ) {
        if (a.ID < b.ID)
            return true;
        else
            return false;
    }

class  scheduling {
private:
    int currentTime ;
	int timeSlice ;
	int method ;
	string inputFileName ;
    vector<Data> workList ;
    vector<resultData> waitingTime ;
    vector<resultData> turnaroundTime ;
    vector<char> FCFSList ;
    vector<char> RRList ;
    vector<char> SRTFList ;
    vector<char> PPRRList ;
    vector<char> HRRNList ;
    queue<Data> readyQueue ;
    vector<Data> readyQueueV ;
public:

    char checkName( int input ) {
        if ( input >= 0 && input <= 9 ) {
            return '0' + input;
        }
        else  {
            return 55+input ;
        }
    }

    void reset(  ) {
        workList.clear();
        waitingTime.clear();
        turnaroundTime.clear();
        FCFSList.clear();
        RRList.clear();
        SRTFList.clear();
        PPRRList.clear();
        HRRNList.clear();
        readyQueueV.clear() ;
    }

    void findShort(  ) {
        workList.clear();
        waitingTime.clear();
        turnaroundTime.clear();
        FCFSList.clear();
        RRList.clear();
        SRTFList.clear();
        PPRRList.clear();
        HRRNList.clear();
        readyQueueV.clear() ;
    }
	bool readFile() {
        int inputNum ;
        Data temp ;
        cout << endl << "Input a file number : " ;
        cin >> inputFileName ;
        string title ;
        fstream theFile;
        theFile.open( inputFileName+".txt" ); // 開啟指定檔案
        int count = 0 ;
        if ( theFile.is_open() ) { // 如果有打開檔案
            theFile >> method ;
            theFile >> timeSlice ; // time slice
            getline( theFile, title ) ; // 讀換行
            getline( theFile, title ) ; // 讀title
            //cout << title  ;
            while (theFile >> inputNum) {
                count ++ ;
                if ( count == 1 ) { // ID
                    temp.ID = inputNum ;
                    temp.name = checkName ( inputNum) ;
                    resultData resultTemp ;
                    resultTemp.ID = inputNum ;
                    waitingTime.push_back( resultTemp) ;
                    turnaroundTime.push_back( resultTemp) ;
                   // cout << temp.ID  << endl ;
                }
                else if ( count == 2) { // CPU Burst
                    temp.CPUBurst = inputNum ;
                    //cout << temp.CPUBurst  << endl ;
                }
                else if ( count == 3 ) { // Arrival Time
                    temp.ArrivalTime = inputNum ;
                   // cout << temp.ArrivalTime  << endl ;
                }
                else if ( count == 4 ) {
                    temp.Priority = inputNum ; // Priority
                    //cout << temp.Priority  << endl ;
                    workList.push_back(temp);
                    count = 0 ;
                }
            }

            theFile.close();
            sort(workList.begin(), workList.end(), sortArrival);
            sort(waitingTime.begin(), waitingTime.end(), sortID);
            /*for( int i = 0 ; i < waitingTime.size() ; i++) {
                cout << waitingTime[i].ID << endl ;
            }*/
            sort(turnaroundTime.begin(), turnaroundTime.end(), sortID);
            if ( method == 1 ) {
                FCFS() ;
                writeFile2() ;
            }
            else if ( method == 2 ) {
                RR() ;
                writeFile2() ;
            }
            else if ( method == 3 ) {
                SRTF() ;
                writeFile2() ;
            }
            else if ( method == 4 ) {
                PPRR() ;
                writeFile2() ;
            }
            else if ( method == 5 ) {
                HRRN() ;
                writeFile2() ;
            }
            else if ( method == 6 ) {
                FCFS() ;
                RR() ;
                SRTF() ;
                PPRR() ;
                HRRN() ;
                writeFile() ;
            }
            return true ;
        } // if
        else {
            cout << "input" +inputFileName + ".txt does not exist!" << endl ;
            return false ;
        }
    }

    void FCFS() {
        int i = workList.size() -1 ;
        Data temp ;
        Data runProcess ;
        currentTime = 0 ;
        bool isDone = false ;
        bool haveTask = false ;
        while ( !isDone ) {
            while ( i >= 0 && currentTime == workList.at(i).ArrivalTime  ) {
                temp = workList[i] ;
                //workList.pop_back() ;
                readyQueue.push(temp) ;
                //cout << temp.ID << "   " << temp.CPUBurst << "   " << temp.ArrivalTime << "   " << temp.Priority<< endl ;
                i-- ;
            } // 將資料放入queue

            currentTime++ ;
            if ( readyQueue.size() != 0 && !haveTask) {
                runProcess = readyQueue.front() ;
                readyQueue.pop() ;
                runProcess.runTime = 0 ;
                haveTask = true ;
            } //如果沒工作，插入下一個工作

            if (  !haveTask) {
                FCFSList.push_back( '-' ) ;
            } // 如果沒有工作
            else {
                FCFSList.push_back( runProcess.name ) ;
                runProcess.runTime++ ;
                if ( runProcess.runTime == runProcess.CPUBurst ) {
                    resultData calTemp ;
                    calTemp.ID = runProcess.ID ;
                    int turnaroundTemp = currentTime - runProcess.ArrivalTime ;
                    for ( int i = 0 ; i < waitingTime.size() ; i++ ) {
                        if ( waitingTime.at(i).ID == runProcess.ID ) {
                            turnaroundTime[i].FCFS = turnaroundTemp ;
                            waitingTime[i].FCFS = turnaroundTemp - runProcess.CPUBurst ;

                        }
                    }

                    /*calTemp.FCFS = turnaroundTemp ;
                    //cout << endl << calTemp.ID<<"          " << calTemp.FCFS << endl ;
                    turnaroundTime.push_back( calTemp ) ;
                    calTemp.FCFS = turnaroundTemp - runProcess.CPUBurst ;
                    waitingTime.push_back( calTemp ) ;
                    //cout << endl << calTemp.ID<<"          " << calTemp.FCFS << endl ;*/
                    haveTask = false ;
                }
                //cout << runProcess.name ;
            }

            if ( i < 0 && readyQueue.size() == 0 && !haveTask)
                isDone = true ;

        } // while

        cout << endl ;
        /*for ( int i = 0 ; i < waitingTime.size() ; i++ )
                        cout << endl << turnaroundTime[i].ID<<"          "<<turnaroundTime[i].FCFS<<"          " << waitingTime[i].FCFS << endl ;*/
        cout  << "==          FCFS==" << endl ;
        for ( int i = 0 ; i < FCFSList.size() ; i++ )
            cout << FCFSList.at(i) ;
    } // FCFS

    void RR () {
        int timeSliceCount = 0 ;
        int i = workList.size() -1 ;
        Data temp ;
        Data runProcess ;
        currentTime = 0 ;
        bool isDone = false ;
        bool haveTask = false ;
        while ( !isDone ) {
            while ( i >= 0 && currentTime == workList.at(i).ArrivalTime  ) {
                temp = workList[i] ;
                readyQueue.push(temp) ;
                i-- ;
            }  // while

            if ( timeSlice == timeSliceCount && haveTask) {
                    //cout << "Put" <<runProcess.ID<< endl ;
                    readyQueue.push(runProcess) ;
                    haveTask = false ;
            } // timeslice 換工作
            currentTime++ ;
            if ( readyQueue.size() != 0 && !haveTask) {
                runProcess = readyQueue.front() ;
                readyQueue.pop() ;
                runProcess.runTime  ;
                timeSliceCount = 0 ;
                haveTask = true ;
            } //如果沒工作，插入下一個工作

            if (  !haveTask) {
                RRList.push_back( '-' ) ;
            } // 如果沒有工作
            else {
                RRList.push_back( runProcess.name ) ;
                runProcess.runTime++ ;
                timeSliceCount++ ;
                if ( runProcess.runTime == runProcess.CPUBurst ) {
                    resultData calTemp ;
                    calTemp.ID = runProcess.ID ;
                    int turnaroundTemp = currentTime - runProcess.ArrivalTime ;
                    for ( int i = 0 ; i < waitingTime.size() ; i++ ) {
                        if ( waitingTime.at(i).ID == runProcess.ID ) {
                            turnaroundTime[i].RR = turnaroundTemp ;
                            waitingTime[i].RR = turnaroundTemp - runProcess.CPUBurst ;

                        }
                    }
                    /*calTemp.RR = turnaroundTemp ;
                    //cout << endl << calTemp.ID<<"          " << calTemp.RR << endl ;
                    turnaroundTime.push_back( calTemp ) ;
                    calTemp.RR = turnaroundTemp - runProcess.CPUBurst ;
                    waitingTime.push_back( calTemp ) ;*/
                    //cout << endl << calTemp.ID<<"          " << calTemp.FCFS << endl ;
                    //cout << "Done" <<runProcess.ID<< endl ;
                    haveTask = false ;
                }

            }

            if ( i < 0 && readyQueue.size() == 0 && !haveTask)
                isDone = true ;

        } // while

        cout << endl ;
        cout  << "==          RR==" << endl ;
        for ( int i = 0 ; i < RRList.size() ; i++ )
            cout << RRList.at(i) ;
    } // RR


    void SRTF() {
        int i = workList.size() -1 ;
        Data temp ;
        Data runProcess ;
        currentTime = 0 ;
        bool isDone = false ;
        bool haveTask = false ;
        while ( !isDone ) {
            while ( i >= 0 && currentTime == workList.at(i).ArrivalTime  ) {
                temp = workList[i] ;
                readyQueueV.insert(readyQueueV.begin(),temp);
                if ( haveTask) {
                    readyQueueV.insert(readyQueueV.begin(),runProcess);
                    haveTask = false ;
                } // 重新放入
                  //  cout << readyQueueV.at(i).ID << "    "<< readyQueueV.at(i).CPUBurst << "    " << readyQueueV.at(i).runTime << endl ;*/
                i-- ;
            } // 將資料放入queue

            currentTime++ ;
            if ( readyQueueV.size() != 0 && !haveTask) {
                Data shortestDate = readyQueueV[readyQueueV.size() -1] ;
                int shortest = readyQueueV.size() -1 ;
                int needTime = readyQueueV.at(readyQueueV.size() -1).CPUBurst - readyQueueV.at(readyQueueV.size() -1).runTime;
                if ( readyQueueV.size() >= 2) {
                    for ( int i = readyQueueV.size() -1 ; i >= 0 ; i-- ) {
                        bool findShortest = false ;
                        if ( needTime > (readyQueueV.at(i).CPUBurst - readyQueueV.at(i).runTime)) {
                            findShortest = true ;
                        }
                        else if ( needTime == (readyQueueV.at(i).CPUBurst - readyQueueV.at(i).runTime) && readyQueueV.at(i).ArrivalTime < shortestDate.ArrivalTime) {
                            findShortest = true ;
                        }
                        else if ( needTime == (readyQueueV.at(i).CPUBurst - readyQueueV.at(i).runTime) && readyQueueV.at(i).ArrivalTime == shortestDate.ArrivalTime &&  readyQueueV.at(i).ID < shortestDate.ID ) {
                            findShortest = true ;
                        }

                        if ( findShortest) {
                            shortest = i ;
                            needTime = readyQueueV.at(i).CPUBurst - readyQueueV.at(i).runTime ;
                            shortestDate = readyQueueV[i] ;
                            Data input ;
                            input = readyQueueV[i] ;
                            readyQueueV.erase(readyQueueV.begin()+i) ;
                            readyQueueV.insert(readyQueueV.end(),input);
                        }

                    } // for

                }

              //  for ( int i = readyQueueV.size() -1 ; i >= 0 ; i-- )
               //     cout << readyQueueV.at(i).ID << "    "<< readyQueueV.at(i).CPUBurst << "    " << readyQueueV.at(i).runTime << endl ;
                runProcess =  readyQueueV.back() ;
                readyQueueV.pop_back() ;
                haveTask = true ;
            } //如果沒工作，插入下一個工作

            if (  !haveTask) {
                SRTFList.push_back( '-' ) ;
            } // 如果沒有工作
            else {
                SRTFList.push_back( runProcess.name ) ;
                runProcess.runTime++ ;
                if ( runProcess.runTime == runProcess.CPUBurst ) {
                    resultData calTemp ;
                    calTemp.ID = runProcess.ID ;
                    int turnaroundTemp = currentTime - runProcess.ArrivalTime ;
                    for ( int i = 0 ; i < waitingTime.size() ; i++ ) {
                        if ( waitingTime.at(i).ID == runProcess.ID ) {
                            turnaroundTime[i].SRTF = turnaroundTemp ;
                            waitingTime[i].SRTF = turnaroundTemp - runProcess.CPUBurst ;

                        }
                    }
                    haveTask = false ;
                }
                //cout << runProcess.name ;
            }

            if ( i < 0 && readyQueueV.size() == 0 && !haveTask)
                isDone = true ;

        } // while

        cout << endl ;
        cout  << "==          SRTF==" << endl ;
        for ( int i = 0 ; i < SRTFList.size() ; i++ )
            cout << SRTFList.at(i) ;
    } // SRTF

    void PPRR() {
        int timeSliceCount = 0 ;
        int i = workList.size() -1 ;
        Data temp ;
        Data runProcess ;
        currentTime = 0 ;
        bool isDone = false ;
        bool haveTask = false ;
        while ( !isDone ) {
            bool newIN ;
            while ( i >= 0 && currentTime == workList.at(i).ArrivalTime  ) {
                temp = workList[i] ;
                readyQueueV.insert(readyQueueV.begin(),temp);
                i-- ;
                if ( haveTask ) {
                    if ( temp.Priority < runProcess.Priority) {
                        haveTask = false ;
                        readyQueueV.insert(readyQueueV.begin(),runProcess);
                    }
                }
            }  // while

            if ( timeSlice == timeSliceCount && haveTask) {
                    readyQueueV.insert(readyQueueV.begin(),runProcess);
                    haveTask = false ;
            } // timeslice 換工作


            if ( readyQueueV.size() != 0 && !haveTask) {
                Data choose = readyQueueV[readyQueueV.size() -1] ;
                int shortest = readyQueueV.size() -1 ;

                if ( readyQueueV.size() >= 2) {
                    for ( int i = readyQueueV.size() -1 ; i >= 0 ; i-- ) {
                        bool findShortest = false ;

                        if ( newIN) {

                        }
                        else if ( choose.Priority > readyQueueV.at(i).Priority ) {
                            findShortest = true ;
                        }

                        if ( findShortest) {
                            shortest = i ;
                            //cout << readyQueueV[i].name<< "     "  << responseRatio << endl ;
                            choose = readyQueueV[i] ;
                            Data input ;
                            input = readyQueueV[i] ;
                            readyQueueV.erase(readyQueueV.begin()+i) ;
                            readyQueueV.insert(readyQueueV.end(),input);
                        }
                    } // for
                }

                runProcess =  readyQueueV.back() ;
                readyQueueV.pop_back() ;
                timeSliceCount = 0 ;
                haveTask = true ;
            } //如果沒工作，插入下一個工作

            currentTime++ ;
            if (  !haveTask) {
                PPRRList.push_back( '-' ) ;
            } // 如果沒有工作
            else {
                PPRRList.push_back( runProcess.name ) ;
                runProcess.runTime++ ;
                timeSliceCount++ ;
                if ( runProcess.runTime == runProcess.CPUBurst ) {
                    resultData calTemp ;
                    calTemp.ID = runProcess.ID ;
                    int turnaroundTemp = currentTime - runProcess.ArrivalTime ;
                    for ( int i = 0 ; i < waitingTime.size() ; i++ ) {
                        if ( waitingTime.at(i).ID == runProcess.ID ) {
                            turnaroundTime[i].PPRR = turnaroundTemp ;
                            waitingTime[i].PPRR = turnaroundTemp - runProcess.CPUBurst ;
                        }
                    }
                    haveTask = false ;
                }
            }

            if ( i < 0 && readyQueueV.size() == 0 && !haveTask)
                isDone = true ;

        } // while

        cout << endl ;
        cout  << "==          PPRR==" << endl ;
        for ( int i = 0 ; i < PPRRList.size() ; i++ )
            cout << PPRRList.at(i) ;
    } // PPRR
        void HRRN() {
        int i = workList.size() -1 ;
        Data temp ;
        Data runProcess ;
        currentTime = 0 ;
        bool isDone = false ;
        bool haveTask = false ;
        while ( !isDone ) {
            while ( i >= 0 && currentTime == workList.at(i).ArrivalTime  ) {
                temp = workList[i] ;
                readyQueueV.insert(readyQueueV.begin(),temp);
                i-- ;
            } // 將資料放入queue
            if ( readyQueueV.size() != 0 && !haveTask) {
                Data choose = readyQueueV[readyQueueV.size() -1] ;
                int shortest = readyQueueV.size() -1 ;
                float responseRatio = ((float)currentTime- (float)choose.ArrivalTime+(float)choose.CPUBurst) / (float)choose.CPUBurst;
                //cout << responseRatio << "  1"<< endl ;
                if ( readyQueueV.size() >= 2) {
                    for ( int i = readyQueueV.size() -1 ; i >= 0 ; i-- ) {
                        bool findShortest = false ;
                        float cmpData = (( (float)currentTime - (float)readyQueueV.at(i).ArrivalTime + (float)readyQueueV.at(i).CPUBurst) / (float)readyQueueV.at(i).CPUBurst) ;
                        if ( responseRatio < cmpData) {
                            findShortest = true ;
                        }
                        else if ( responseRatio == cmpData && readyQueueV.at(i).ArrivalTime < choose.ArrivalTime) {
                            findShortest = true ;
                        }
                        else if ( responseRatio == cmpData  && readyQueueV.at(i).ArrivalTime == choose.ArrivalTime &&  readyQueueV.at(i).ID < choose.ID ) {
                            findShortest = true ;
                        }
                        if ( findShortest) {
                            shortest = i ;
                            responseRatio = cmpData ;
                            //cout << readyQueueV[i].name<< "     "  << responseRatio << endl ;
                            choose = readyQueueV[i] ;
                            Data input ;
                            input = readyQueueV[i] ;
                            readyQueueV.erase(readyQueueV.begin()+i) ;
                            readyQueueV.insert(readyQueueV.end(),input);
                        }
                    } // for
                }
                runProcess =  readyQueueV.back() ;
                readyQueueV.pop_back() ;
                haveTask = true ;
            } //如果沒工作，插入下一個工作
            currentTime++ ;
            if (  !haveTask) {
                HRRNList.push_back( '-' ) ;
            } // 如果沒有工作
            else {
                HRRNList.push_back( runProcess.name ) ;
                runProcess.runTime++ ;
                if ( runProcess.runTime == runProcess.CPUBurst ) {
                    resultData calTemp ;
                    calTemp.ID = runProcess.ID ;
                    int turnaroundTemp = currentTime - runProcess.ArrivalTime ;
                    for ( int i = 0 ; i < waitingTime.size() ; i++ ) {
                        if ( waitingTime.at(i).ID == runProcess.ID ) {
                            turnaroundTime[i].HRRN = turnaroundTemp ;
                            waitingTime[i].HRRN = turnaroundTemp - runProcess.CPUBurst ;
                        }
                    }
                    haveTask = false ;
                }
                //cout << runProcess.name ;
            }
            if ( i < 0 && readyQueueV.size() == 0 && !haveTask)
                isDone = true ;
        } // while
        cout << endl ;
        cout  << "==          HRRN==" << endl ;
        for ( int i = 0 ; i < HRRNList.size() ; i++ )
            cout << HRRNList.at(i) ;
    } // HRRN

    void writeFile() {
        fstream newFile;
        newFile.open("out_"+inputFileName+".txt", ios::out);
        int count = 0 ;
        newFile << "All" << endl ;
        newFile << "==        FCFS==" << endl ;
        for ( int i = 0 ; i < FCFSList.size() ; i++ )
            newFile << FCFSList.at(i) ;
        newFile << endl << "==          RR==" << endl ;
        for ( int i = 0 ; i < RRList.size() ; i++ )
            newFile << RRList.at(i) ;
        newFile <<  endl << "==        SRTF==" << endl ;
        for ( int i = 0 ; i < SRTFList.size() ; i++ )
            newFile << SRTFList.at(i) ;
        newFile <<  endl << "==        PPRR==" << endl ;
        for ( int i = 0 ; i < PPRRList.size() ; i++ )
            newFile << PPRRList.at(i) ;
        newFile <<  endl << "==        HRRN==" << endl ;
        for ( int i = 0 ; i < HRRNList.size() ; i++ )
            newFile << HRRNList.at(i) ;
        newFile << endl<<"===========================================================" << endl << endl  ;
        newFile << "Waiting Time" << endl ;
        newFile << "ID\tFCFS\tRR\tSRTF\tPPRR\tHRRN" << endl ;
        newFile << "===========================================================" << endl ;
        cout << endl ;
        while ( count < waitingTime.size()) {
                newFile << waitingTime.at(count).ID << "\t" << waitingTime.at(count).FCFS << "\t" << waitingTime.at(count).RR << "\t" << waitingTime.at(count).SRTF << "\t" << waitingTime.at(count).PPRR << "\t" << waitingTime.at(count).HRRN << endl ;
                count ++ ;
        }
        newFile << "==========================================================="  <<endl ;
        newFile  << endl<< "Turnaround Time" << endl ;
        newFile << "ID\tFCFS\tRR\tSRTF\tPPRR\tHRRN" << endl ;
        newFile << "===========================================================" << endl ;
        count = 0 ;
        while ( count < turnaroundTime.size()) {
                newFile << turnaroundTime.at(count).ID << "\t" << turnaroundTime.at(count).FCFS << "\t" << turnaroundTime.at(count).RR << "\t" << turnaroundTime.at(count).SRTF << "\t" << turnaroundTime.at(count).PPRR << "\t" << turnaroundTime.at(count).HRRN << endl ;
                count ++ ;
        }
        newFile << "===========================================================" << endl << endl  ;
        newFile.close();
    } // writeFile()

    void writeFile2() {
        fstream newFile;
        newFile.open("out_"+inputFileName+".txt", ios::out);
        int count = 0 ;
        if ( method == 1 ) {
                newFile << "FCFS" << endl ;
                newFile << "==        FCFS==" << endl ;
                for ( int i = 0 ; i < FCFSList.size() ; i++ )
                    newFile << FCFSList.at(i) ;
                newFile << endl<<"===========================================================" << endl << endl  ;
                newFile << "Waiting Time" << endl ;
                newFile << "ID\tFCFS" << endl ;
                newFile << "===========================================================" << endl ;
                cout << endl ;
                while ( count < waitingTime.size()) {
                        newFile << waitingTime.at(count).ID << "\t" << waitingTime.at(count).FCFS << endl ;
                        count ++ ;
                }
                newFile << "==========================================================="  <<endl ;
                newFile  << endl<< "Turnaround Time" << endl ;
                newFile << "ID\tFCFS" << endl ;
                newFile << "===========================================================" << endl ;
                count = 0 ;
                while ( count < turnaroundTime.size()) {
                        newFile << turnaroundTime.at(count).ID << "\t" << turnaroundTime.at(count).FCFS<< endl ;
                        count ++ ;
                }
                newFile << "===========================================================" << endl << endl  ;
        }
        else if ( method == 2 ) {
                newFile << "RR" << endl ;
                newFile << "==          RR==" << endl ;
                for ( int i = 0 ; i < RRList.size() ; i++ )
                    newFile << RRList.at(i) ;
                newFile << endl<<"===========================================================" << endl << endl  ;
                newFile << "Waiting Time" << endl ;
                newFile << "ID\tRR" << endl ;
                newFile << "===========================================================" << endl ;
                cout << endl ;
                while ( count < waitingTime.size()) {
                        newFile << waitingTime.at(count).ID << "\t" << waitingTime.at(count).RR << endl ;
                        count ++ ;
                }
                newFile << "==========================================================="  <<endl ;
                newFile  << endl<< "Turnaround Time" << endl ;
                newFile << "ID\tRR" << endl ;
                newFile << "===========================================================" << endl ;
                count = 0 ;
                while ( count < turnaroundTime.size()) {
                        newFile << turnaroundTime.at(count).ID << "\t" << turnaroundTime.at(count).RR<< endl ;
                        count ++ ;
                }
                newFile << "===========================================================" << endl << endl  ;
        }
        else if ( method == 3 ) {
            newFile << "SRTF" << endl ;
                newFile << "==        SRTF==" << endl ;
                for ( int i = 0 ; i < SRTFList.size() ; i++ )
                    newFile << SRTFList.at(i) ;
                newFile << endl<<"===========================================================" << endl << endl  ;
                newFile << "Waiting Time" << endl ;
                newFile << "ID\tSRTF" << endl ;
                newFile << "===========================================================" << endl ;
                cout << endl ;
                while ( count < waitingTime.size()) {
                        newFile << waitingTime.at(count).ID << "\t" << waitingTime.at(count).SRTF << endl ;
                        count ++ ;
                }
                newFile << "==========================================================="  <<endl ;
                newFile  << endl<< "Turnaround Time" << endl ;
                newFile << "ID\tSRTF" << endl ;
                newFile << "===========================================================" << endl ;
                count = 0 ;
                while ( count < turnaroundTime.size()) {
                        newFile << turnaroundTime.at(count).ID << "\t" << turnaroundTime.at(count).SRTF<< endl ;
                        count ++ ;
                }
                newFile << "===========================================================" << endl << endl  ;
        }
        else if ( method == 4 ) {
            newFile << "Priority RR" << endl ;
                newFile << "==        PPRR==" << endl ;
                for ( int i = 0 ; i < PPRRList.size() ; i++ )
                    newFile << PPRRList.at(i) ;
                newFile << endl<<"===========================================================" << endl << endl  ;
                newFile << "Waiting Time" << endl ;
                newFile << "ID\tPPRR" << endl ;
                newFile << "===========================================================" << endl ;
                cout << endl ;
                while ( count < waitingTime.size()) {
                        newFile << waitingTime.at(count).ID << "\t" << waitingTime.at(count).PPRR << endl ;
                        count ++ ;
                }
                newFile << "==========================================================="  <<endl ;
                newFile  << endl<< "Turnaround Time" << endl ;
                newFile << "ID\tPPRR" << endl ;
                newFile << "===========================================================" << endl ;
                count = 0 ;
                while ( count < turnaroundTime.size()) {
                        newFile << turnaroundTime.at(count).ID << "\t" << turnaroundTime.at(count).PPRR<< endl ;
                        count ++ ;
                }
                newFile << "===========================================================" << endl << endl  ;
        }
        else if ( method == 5 ) {
            newFile << "HRRN" << endl ;
                newFile << "==        HRRN==" << endl ;
                for ( int i = 0 ; i < HRRNList.size() ; i++ )
                    newFile << HRRNList.at(i) ;
                newFile << endl<<"===========================================================" << endl << endl  ;
                newFile << "Waiting Time" << endl ;
                newFile << "ID\tHRRN" << endl ;
                newFile << "===========================================================" << endl ;
                cout << endl ;
                while ( count < waitingTime.size()) {
                        newFile << waitingTime.at(count).ID << "\t" << waitingTime.at(count).HRRN << endl ;
                        count ++ ;
                }
                newFile << "==========================================================="  <<endl ;
                newFile  << endl<< "Turnaround Time" << endl ;
                newFile << "ID\tHRRN" << endl ;
                newFile << "===========================================================" << endl ;
                count = 0 ;
                while ( count < turnaroundTime.size()) {
                        newFile << turnaroundTime.at(count).ID << "\t" << turnaroundTime.at(count).HRRN << endl ;
                        count ++ ;
                }
                newFile << "===========================================================" << endl << endl  ;
        }

        newFile.close();
    } // writeFile()
};
int main(void) {
    scheduling s;
    bool haveFile ;
	int command = -1;
	  do {
        cout << endl << "***     Scheduling      ***" ;
        cout << endl << "* 0. Quit                 *" ;
        cout << endl << "* 1. CPU Scheduling       *" ;
        //cout << endl << "* 2. RR                   *" ;
       // cout << endl << "* 3. SRTF                 *" ;
        //cout << endl << "* 4. PPRR                 *" ;
        //cout << endl << "* 5. HRRN                 *" ;
        //cout << endl << "* 6. ALL                  *" ;
        cout << endl << "***************************" ;
        cout << endl << "Input a command( 0, 1 ) : " ;
        cin >> command ; // get the command

        switch(command){
            case 0 :
                break ;
            case 1 :
                s.reset() ;
                s.readFile() ;
                break ;
            default :
			cout << endl << "command does not exist !!" << endl ;
                break ;

	} // end switch

  } while ( command != 0 ) ;

}
