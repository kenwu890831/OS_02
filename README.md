# OS_02

# 實作方法和流程 
➢ 程式開始執行時，輸出使用者操作介面以方便使用者選
擇CPU排程的方式，可重複執行直到輸入0退出。 
➢ 根據使用者輸入的檔案名稱進行操作，使先檢查是否有
對應的檔案，有的話繼續執行程式，沒有的話輸出檔案
不存在的提示，並且重新輸入新的排程方式。 
➢ 確認正確讀檔之後，進行排程，並且計算Turnaround 
Time 和 Waiting Time。 
➢ 將執行完的資料寫入新的檔案，依照範例格式寫入，寫
入完成跳回使用者介面，繼續執行直到輸入0。 

# 不同排程法的比較 
##  Average Waiting Time 
### Input1(15 筆資料) 
* FCFS : 14.333 
* RR : 18.4 
* SRTF : 8.066 (Average Waiting Time 最短) 
* PPRR : 14.666 
* HRRN : 11.6 
### Input2(5 筆資料) 
* FCFS : 8.4 
* RR : 6.4 
* SRTF : 1 (Average Waiting Time最短) 
* PPRR : 9.4 
* HRRN : 8.2 
### Input3(6 筆資料) 
* FCFS : 6.666 (Average Waiting Time 最短) 
* RR : 11.666 
* SRTF : 6.666 (Average Waiting Time 最短) 
* PPRR : 12.5 
* HRRN : 6.666 (Average Waiting Time 最短) 
➢ Average Turnaround Time 
### Input1(15 筆資料) 
* FCFS : 18.2 
* RR : 22.267 
* SRTF : 11.933 (Average Turnaround Time 最短) 
* PPRR : 18.533 
* HRRN : 15.466 
### Input2 
* FCFS : 13.2 
* RR : 11.2 
* SRTF : 7.8 (Average Turnaround Time最短) 
* PPRR : 14.2 
* HRRN : 13 
### Input3 
* FCFS : 24.166 (Average Turnaround Time 最短) 
* RR : 29.166 
* SRTF : 24.166 (Average Turnaround Time 最短) 
* PPRR : 30 
* HRRN : 24.166 (Average Turnaround Time 最短) 

# 結果與討論 
從統計的結果能夠知道SRTF的Average Waiting Time
以及Average Turnaround Time 會是最短的，是比較好
的一個排程方法，其他的排成方法受到資料的影響比較
大，但是在使用使用上我們仍然會使用各種排程法，不
單單只使用SRTF，因為各排程法在每個事情上仍然有各
自的優勢在，所以每個排程法我們仍然需要了結其運作
方式。 
