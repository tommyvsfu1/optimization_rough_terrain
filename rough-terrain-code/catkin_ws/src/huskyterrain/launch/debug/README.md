huskytraver_for_sampling_wrong_point.launch

順序
sampling.py -> huskytraver_for_sampling_wrong_point.launch -> traversability_write -> simple_goal.cpp
這是為了debug wrong point 的問題, 因為control flow 邏輯不對, 或是 publish 跟thread 之間沒有處理好造成你沒有拿到已更新的值
