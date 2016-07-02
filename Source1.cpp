


#include<DxLib.h>
#include<math.h>
#include<stdlib.h>

#define PI 3.14159265f


int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow )
{
	ChangeWindowMode( TRUE );
	SetGraphMode( 800 , 600 , 32 );
	
	SetOutApplicationLogValidFlag( FALSE ) ;//logを生成しないように設定
	if(DxLib_Init() == 1)//ここではDxLibの初期化に失敗した時にエラーを吐かせて終了させてます
	{
		return -1;
	}

	SetDrawScreen( DX_SCREEN_BACK ) ;//描画先を裏画面に指定。ダブルバッファリングの大事な所です

	int mode=1;

	//キー状態
	int getkey[300] = {0};

	int i , j;
	int wh = GetColor( 255 , 255 , 255 )  ;
	
	double x0 =300;
	double y0 =300;
	

	//マウス座標
	int mousex , mousey;
	

	//画像読み込み
	int GHandle;
	GHandle = LoadGraph("tate.png");

	double x[1000] , y[1000] , vx[1000] , vy[1000] , xd[1000], yd[1000];

	for(i=0 ; i<1000 ; i++){
		x[i] = rand()%800;
		y[i] = rand()%600;
		xd[i] =0;
		yd[i] =0;
		vx[i] = 0;
		vy[i] = 0;

	}
	x[0] = 400;
	y[0] = 300;

	x[900] = 400;
	y[900] = 300;

	double rad = 0;



	while( ProcessMessage()==0)
	{
		ClearDrawScreen();//ここで裏画面に描画していたものを消してます
		

		//マウス座標取得
		GetMousePoint( &mousex, &mousey ); 
		

		SetDrawBlendMode( DX_BLENDMODE_ALPHA , 240 ) ;
		SetDrawBright( 255,255,255 ) ;
		DrawString( 20 , 20 , "Zで引力、Xで斥力、Cで目標点変更、Aで各点を結ぶ" , wh ) ;


		
			

		for(i=0 ; i<900 ; i++){
			double x1 = x[i]  , y1 = y[i] , x2 = x[i+1] , y2 = y[i+1];

			//次の点への距離を測る→距離に応じて速度を設定（単振動）
			double dist = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) ); 
			double vv = 0.5 * dist;
			if(vv>2){vv=2;}
			
			//つぎの点に向けて単振動
			rad = atan2(y2-y1 , x2-x1);
			vx[i] += vv * cos(rad);
			vy[i] += vv * sin(rad);

			//つぎの点を自分に向けて加速（調整。なくても同じような挙動を示す）
			vx[i+1] -= 0.1*vv * cos(rad);
			vy[i+1] -= 0.1*vv * sin(rad);

			
			//減速
			vx[i] *= 0.8;
			vy[i] *= 0.8;
			//マウスに向けて加速
			if(CheckHitKey( KEY_INPUT_Z ) == 1 )
			{
				double rad = atan2(mousey-y[i],mousex-x[i] );
				vx[i] += 0.5*cos(rad);
				vy[i] += 0.5*sin(rad);
			}
			//マウス逆方向に向けて加速
			if(CheckHitKey( KEY_INPUT_X ) == 1 )
			{
				double rad = atan2(mousey-y[i],mousex-x[i] );
				vx[i] -= 0.5*cos(rad);
				vy[i] -= 0.5*sin(rad);
			}
		}


		//画面端で反射
		for(i=1 ; i<901 ; i++){
			if(vx[i]>0 && x[i]>800){vx[i] *= -1;}
			if(vx[i]<0 && x[i]<0){vx[i] *= -1;}
			if(vy[i]>0 && y[i]>600){vy[i] *= -1;}
			if(vy[i]<0 && y[i]<0){vy[i] *= -1;}
		}

		//移動
		for(i=0 ; i<900 ; i++){
			x[i] += vx[i];
			y[i] += vy[i];
		}
			



		//以下描画

		for(i=1 ; i<900-11 ; i++){
			//座標を取得
			double x1 = x[i] , y1 = y[i] , 
				   x2 = x[i+10] , y2 = y[i+10] ,
				   x3 = x[i+1] , y3 = y[i+1] , 
				   x4 = x[i+11] , y4 = y[i+11] ;

			//透明度と色を指定(赤→オレンジに）
			SetDrawBlendMode( DX_BLENDMODE_ALPHA , 30 ) ;
			SetDrawBright( 255 , i*0.15 , 0 ) ;

			//自由変形して描画
			DrawModiGraph( x1 , y1 , x2 , y2 ,
						   x4 , y4 , x3 , y3 , GHandle , TRUE ) ;

			//Aキーが押されていれば線を描画
			if(CheckHitKey( KEY_INPUT_A ) == 1 )
			{
				SetDrawBlendMode( DX_BLENDMODE_ALPHA , 255 ) ;
				SetDrawBright( 255,255,255 ) ;
				DrawLine(x[i], y[i], x[i+1] , y[i+1] , GetColor(0,0,255));
			}
		}




		//目標点の十字を描画
		SetDrawBlendMode( DX_BLENDMODE_ALPHA , 100 ) ;
		SetDrawBright( 255,0,0 ) ;
		DrawLine(x[900]-100, y[900], x[900]+100 , y[900] , wh);
		DrawLine(x[900], y[900]-100, x[900] , y[900]+100 , wh);
			

		//Cが押されれば目標点変更
		if(CheckHitKey( KEY_INPUT_C ) == 1 )
		{	
			x[900] = mousex;
			y[900] = mousey;
		}


		ScreenFlip();//ここで裏画面を表画面にコピーしてます
	}
	DxLib_End();
return 0;
}
	
