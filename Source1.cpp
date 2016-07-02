


#include<DxLib.h>
#include<math.h>
#include<stdlib.h>

#define PI 3.14159265f


int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow )
{
	ChangeWindowMode( TRUE );
	SetGraphMode( 800 , 600 , 32 );
	
	SetOutApplicationLogValidFlag( FALSE ) ;//log�𐶐����Ȃ��悤�ɐݒ�
	if(DxLib_Init() == 1)//�����ł�DxLib�̏������Ɏ��s�������ɃG���[��f�����ďI�������Ă܂�
	{
		return -1;
	}

	SetDrawScreen( DX_SCREEN_BACK ) ;//�`���𗠉�ʂɎw��B�_�u���o�b�t�@�����O�̑厖�ȏ��ł�

	int mode=1;

	//�L�[���
	int getkey[300] = {0};

	int i , j;
	int wh = GetColor( 255 , 255 , 255 )  ;
	
	double x0 =300;
	double y0 =300;
	

	//�}�E�X���W
	int mousex , mousey;
	

	//�摜�ǂݍ���
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
		ClearDrawScreen();//�����ŗ���ʂɕ`�悵�Ă������̂������Ă܂�
		

		//�}�E�X���W�擾
		GetMousePoint( &mousex, &mousey ); 
		

		SetDrawBlendMode( DX_BLENDMODE_ALPHA , 240 ) ;
		SetDrawBright( 255,255,255 ) ;
		DrawString( 20 , 20 , "Z�ň��́AX�Ő˗́AC�ŖڕW�_�ύX�AA�Ŋe�_������" , wh ) ;


		
			

		for(i=0 ; i<900 ; i++){
			double x1 = x[i]  , y1 = y[i] , x2 = x[i+1] , y2 = y[i+1];

			//���̓_�ւ̋����𑪂遨�����ɉ����đ��x��ݒ�i�P�U���j
			double dist = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) ); 
			double vv = 0.5 * dist;
			if(vv>2){vv=2;}
			
			//���̓_�Ɍ����ĒP�U��
			rad = atan2(y2-y1 , x2-x1);
			vx[i] += vv * cos(rad);
			vy[i] += vv * sin(rad);

			//���̓_�������Ɍ����ĉ����i�����B�Ȃ��Ă������悤�ȋ����������j
			vx[i+1] -= 0.1*vv * cos(rad);
			vy[i+1] -= 0.1*vv * sin(rad);

			
			//����
			vx[i] *= 0.8;
			vy[i] *= 0.8;
			//�}�E�X�Ɍ����ĉ���
			if(CheckHitKey( KEY_INPUT_Z ) == 1 )
			{
				double rad = atan2(mousey-y[i],mousex-x[i] );
				vx[i] += 0.5*cos(rad);
				vy[i] += 0.5*sin(rad);
			}
			//�}�E�X�t�����Ɍ����ĉ���
			if(CheckHitKey( KEY_INPUT_X ) == 1 )
			{
				double rad = atan2(mousey-y[i],mousex-x[i] );
				vx[i] -= 0.5*cos(rad);
				vy[i] -= 0.5*sin(rad);
			}
		}


		//��ʒ[�Ŕ���
		for(i=1 ; i<901 ; i++){
			if(vx[i]>0 && x[i]>800){vx[i] *= -1;}
			if(vx[i]<0 && x[i]<0){vx[i] *= -1;}
			if(vy[i]>0 && y[i]>600){vy[i] *= -1;}
			if(vy[i]<0 && y[i]<0){vy[i] *= -1;}
		}

		//�ړ�
		for(i=0 ; i<900 ; i++){
			x[i] += vx[i];
			y[i] += vy[i];
		}
			



		//�ȉ��`��

		for(i=1 ; i<900-11 ; i++){
			//���W���擾
			double x1 = x[i] , y1 = y[i] , 
				   x2 = x[i+10] , y2 = y[i+10] ,
				   x3 = x[i+1] , y3 = y[i+1] , 
				   x4 = x[i+11] , y4 = y[i+11] ;

			//�����x�ƐF���w��(�ԁ��I�����W�Ɂj
			SetDrawBlendMode( DX_BLENDMODE_ALPHA , 30 ) ;
			SetDrawBright( 255 , i*0.15 , 0 ) ;

			//���R�ό`���ĕ`��
			DrawModiGraph( x1 , y1 , x2 , y2 ,
						   x4 , y4 , x3 , y3 , GHandle , TRUE ) ;

			//A�L�[��������Ă���ΐ���`��
			if(CheckHitKey( KEY_INPUT_A ) == 1 )
			{
				SetDrawBlendMode( DX_BLENDMODE_ALPHA , 255 ) ;
				SetDrawBright( 255,255,255 ) ;
				DrawLine(x[i], y[i], x[i+1] , y[i+1] , GetColor(0,0,255));
			}
		}




		//�ڕW�_�̏\����`��
		SetDrawBlendMode( DX_BLENDMODE_ALPHA , 100 ) ;
		SetDrawBright( 255,0,0 ) ;
		DrawLine(x[900]-100, y[900], x[900]+100 , y[900] , wh);
		DrawLine(x[900], y[900]-100, x[900] , y[900]+100 , wh);
			

		//C���������ΖڕW�_�ύX
		if(CheckHitKey( KEY_INPUT_C ) == 1 )
		{	
			x[900] = mousex;
			y[900] = mousey;
		}


		ScreenFlip();//�����ŗ���ʂ�\��ʂɃR�s�[���Ă܂�
	}
	DxLib_End();
return 0;
}
	
