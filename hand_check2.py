#!/usr/bin/env python3

from array import array
import cv2
from cv2 import FONT_HERSHEY_COMPLEX
from cv2 import FONT_HERSHEY_SIMPLEX
import mediapipe as mp
import time
import rospy
import numpy as np
from std_msgs.msg import Int32

mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands

def status(input):
  pub.publish(input)

# For webcam input:
cap = cv2.VideoCapture(2) 

# ros 설정
pub = rospy.Publisher('status_hand', Int32, queue_size=10)
rospy.init_node('talker', anonymous=True)
rate = rospy.Rate(10) # 10

handID = [[20,18],[16,14],[12,10],[8,6],[4,3]]  # 각 손가락 끝점과 그 아랫점의 ID 리스트
number = [0,0,0]
count = 0   # 엄지 손 몇번
fingerCount = 0  # 손가락 개수 세기
isNext = True # 계속 인식하고 있음을 방지하기 위한 Bool 변수

rate = rospy.Rate(10) # 10hz

# 좌표
org1 = (30, 150)
org2 = (40, 300)

# 카메라 캘리브레이션 값
mtx = np.array([395.2936354279367, 0, 299.7445141980046, 0, 515.9782089491932, 278.1818678195682, 0, 0, 1])
mtx.shape=(3,3)

print(mtx)

dist = np.array([-0.2148206684979024, -0.1411513027591933, -0.01677953809712869, 0.0008347761060968042, 0.2133678086528303])

with mp_hands.Hands(max_num_hands=1, min_detection_confidence=0.7, min_tracking_confidence=0.5) as hands:
  while not rospy.is_shutdown():
    success, img = cap.read()
      
    if not success:
      print("Ignoring empty camera frame.")
      # If loading a video, use 'break' instead of 'continue'.
      continue
    
    # 카메라 캘리브레이션
    
    h, w = img.shape[:2]
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(mtx, dist, (w, h), 0, (w,h))
    # image = cv2.undistort(img, mtx, dist, None, newcameramtx)

    mapx,mapy = cv2.initUndistortRectifyMap(mtx,dist,None,mtx,(w,h),5)
    dst = cv2.remap(img,mapx,mapy,cv2.INTER_LINEAR)
    x,y,w,h = roi
    image = dst[y:y+h, x:x+w]


    # Flip the image horizontally for a later selfie-view display, and convert
    # the BGR image to RGB.
    image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)
    # To improve performance, optionally mark the image as not writeable to
    # pass by reference.
    image.flags.writeable = False
    results = hands.process(image)

    # Draw the hand annotations on the image.
    image.flags.writeable = True
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    if results.multi_hand_landmarks: # 손을 감지했을 때
      for hand_landmarks in results.multi_hand_landmarks:
        
        # handID = [[20,18],[16,14],[12,10],[8,6],[4,3]]를 for 문으로 하나씩 돌아가면서 각 손가락을 확인
        for finger in handID: 
            
          # 엄지손가락을 확인하고 있을 때
          if finger[0] == mp_hands.HandLandmark.THUMB_TIP : 
            
            # 엄지손가락이 펴져 있을 때 (왼손)
            if (hand_landmarks.landmark[finger[0]].x >= hand_landmarks.landmark[finger[1]].x): 
              if isNext: # 입력하기 위한 조건문 확인
                number[count] = fingerCount
                count += 1
                print(fingerCount) # 터미널 창에 출력

                text1 = "%s" % fingerCount
                text2 = "%s" % count

                isNext = False
                
                              
                

                # 총 세 번 인식했을 때
                if count == 3:
                  sumNumber = 100*number[0]+10*number[1]+number[2] # 각 리스트의 숫자를 100의 자릿수로 합치기
                    
                  pub.publish(sumNumber) # ros 퍼블리쉬
                  rate.sleep()

                  text2 = "Final!!"
                  text1 = "%s" % sumNumber
                    
                  # 초기화
                  number = [0,0,0]
                  count = 0
                  print("Result = ",sumNumber)
              
              # 화면에 출력
              cv2.putText(image, text1, org1, FONT_HERSHEY_SIMPLEX, 3, (0, 127, 255), 3) 
              cv2.putText(image, text2, org2, FONT_HERSHEY_SIMPLEX, 3, (0, 127, 255), 3)


            # elif (hand_landmarks.landmark[finger[0]].x >= hand_landmarks.landmark[finger[1]].x) and not isFold:
            #   fingerCount += 1
            #   isNext = True
            #   print(fingerCount)

            # elif hand_landmarks.landmark[finger[0]].x < hand_landmarks.landmark[finger[1]].x and isFold:
            #   fingerCount = 0
            #   isNext = True
            #   print(fingerCount)

            # 엄지손가락이 펴져 있지 않을 때
            else:
              print(fingerCount) # 손가락 개수 출력

              text1 = "%s" % fingerCount
              text2 = "%s" % count

              # # 화면상에 출력
              cv2.putText(image, text1, org1, FONT_HERSHEY_SIMPLEX, 3, (0, 127, 255), 3) 
              cv2.putText(image, text2, org2, FONT_HERSHEY_SIMPLEX, 3, (0, 127, 255), 3)
              

              isNext = True # 입력하기 위한 조건문 True값으로 설정

            # 엄지손가락을 인식한 다음은 손가락 개수를 0으로 초기화 함
            fingerCount = 0

          # 엄지손가락이 아닌 다른 손가락을 인식하고 있을 때
          else:

            # 손가락 끝 점이 손가락의 두 번째 관절보다 더 아래에 있으면 굽힌 것으로 판단
            if hand_landmarks.landmark[finger[0]].y < hand_landmarks.landmark[finger[1]].y:
              fingerCount += 1

          # text1 = "%s" % fingerCount
          # cv2.putText(image, text1, org1, FONT_HERSHEY_COMPLEX, 5, (0, 255, 255), 3)
          

        # #검지가 손목보다 위에 있고 엄지가 중지보다 아래에 있을 때
        # if (hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP].y <     # 검지손가락의 y좌표
        #     hand_landmarks.landmark[mp_hands.HandLandmark.WRIST].y                  # 손목 y좌표
        # and hand_landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP].y >            # 엄지손가락 y좌표
        #     hand_landmarks.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_TIP].y):     # 검지손가락 y좌표
        #   status("Front")
        #   print("Front")
          
        # #검지가 손목보다 아래에 있고 엄지가 검지손가락보다 위에 있을 때
        # elif (hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP].y >   # 검지손가락의 y좌표
        #       hand_landmarks.landmark[mp_hands.HandLandmark.WRIST].y                # 손목 y좌표
        #   and hand_landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP].y <          # 엄지손가락 y좌표
        #       hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP].y):   # 검지손가락의 y좌표
        #   status("Back")
        #   print("Back")

        # #검지가 손목보다 왼쪽에 있고 엄지가 검지보다 위에 있을 때
        # elif (hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP].x <   # 검지손가락의 y좌표
        #       hand_landmarks.landmark[mp_hands.HandLandmark.WRIST].x                # 손목 y좌표
        #   and hand_landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP].y <          # 엄지손가락 y좌표
        #       hand_landmarks.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_TIP].y):   # 검지손가락의 y좌표
        #   status("Left")
        #   print("Left")

        # #검지가 손목보다 오른쪽에 있고 엄지가 중지보다 위에 있을 때
        # elif (hand_landmarks.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_TIP].x >   # 검지손가락의 y좌표
        #       hand_landmarks.landmark[mp_hands.HandLandmark.WRIST].x                # 손목 y좌표
        #   and hand_landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP].y <          # 엄지손가락 y좌표
        #       hand_landmarks.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_TIP].y):   # 검지손가락의 y좌표
        #   status("Right")
        #   print("Right")
        # else:
        #   status("s")
        #   print("s")        
          
        # 손 그림 그리기 (예쁘게)
        mp_drawing.draw_landmarks(image, hand_landmarks, mp_hands.HAND_CONNECTIONS, 
                                  mp_drawing_styles.get_default_hand_landmarks_style(),
                                  mp_drawing_styles.get_default_hand_connections_style())
          
    cv2.imshow('hand control', image)
    if cv2.waitKey(5) & 0xFF == 27:
      break
  cap.release()