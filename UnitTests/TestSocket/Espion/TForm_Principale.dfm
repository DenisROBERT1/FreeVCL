object Form_Principale: TForm_Principale
  Left = 373
  Top = 297
  Width = 450
  Height = 426
  Caption = 'Espion socket'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxParamServer: TGroupBox
		Left = 4
		Top = 2
		Width = 432
		Height = 134
		Anchors = [akLeft, akTop, akRight]
		Caption = 'Paramètres serveur'
		TabStop = True
		TabOrder = 0
		object LabelStatusServer: TLabel
			Left = 12
			Top = 18
			Width = 85
			Height = 13
			Caption = 'Etat :'
		end
		object LabelStatusContentServer: TLabel
			Left = 108
			Top = 18
			Width = 300
			Height = 13
			Anchors = [akLeft, akTop, akRight]
			Caption = 'D'#233'connect'#233
		end
		object LabelHostServer: TLabel
			Left = 12
			Top = 44
			Width = 85
			Height = 13
			Caption = 'Host ou IP :'
		end
		object EditHostServer: TEdit
			Left = 108
			Top = 42
			Width = 300
			Height = 21
			Anchors = [akLeft, akTop, akRight]
			TabOrder = 0
			Text = ''
		end
		object LabelPortServer: TLabel
			Left = 12
			Top = 70
			Width = 85
			Height = 13
			Caption = 'Port :'
		end
		object EditPortServer: TEdit
			Left = 108
			Top = 68
			Width = 300
			Height = 21
			Anchors = [akLeft, akTop, akRight]
			TabOrder = 1
			Text = ''
		end
		object Button_ConnecterServer: TButton
			Left = 108
			Top = 100
			Width = 85
			Height = 22
			Caption = 'Connecter'
			TabOrder = 2
			OnClick = Button_ConnecterServerClick
		end
	end
  object GroupBoxParamClient: TGroupBox
		Left = 4
		Top = 136
		Width = 432
		Height = 108
		Anchors = [akLeft, akTop, akRight]
		Caption = 'Paramètres client'
		TabStop = True
		TabOrder = 0
		object LabelStatusClient: TLabel
			Left = 12
			Top = 18
			Width = 85
			Height = 13
			Caption = 'Etat :'
		end
		object LabelStatusContentClient: TLabel
			Left = 108
			Top = 18
			Width = 300
			Height = 13
			Anchors = [akLeft, akTop, akRight]
	    Caption = 'D'#233'sactiv'#233
		end
		object LabelPortClient: TLabel
			Left = 12
			Top = 44
			Width = 85
			Height = 13
			Caption = 'Port :'
		end
		object EditPortClient: TEdit
			Left = 108
			Top = 42
			Width = 300
			Height = 21
			Anchors = [akLeft, akTop, akRight]
			TabOrder = 1
			Text = ''
		end
		object Button_ActiverClient: TButton
			Left = 108
			Top = 74
			Width = 85
			Height = 22
			Caption = 'Activer'
			TabOrder = 2
			OnClick = Button_ActiverClientClick
		end
	end
  object Memo: TMemo
    Left = 4
    Top = 248
    Width = 432
    Height = 146
    TabOrder = 1
    Anchors = [akLeft, akTop, akRight, akBottom]
  end
  object ClientSocket: TClientSocket
    Active = False
    Address = '127.0.0.1'
    ClientType = ctNonBlocking
    Host = 'localhost'
    Port = 8080
    OnConnecting = ClientSocketConnecting
    OnConnect = ClientSocketConnect
    OnDisconnect = ClientSocketDisconnect
    OnRead = ClientSocketRead
    OnError = SocketError
    Left = 402
    Top = 3
  end
  object ServerSocket: TServerSocket
    Active = False
    Port = 8081
    ServerType = stNonBlocking
    OnAccept = ServerSocketAccept
    OnConnect = ServerSocketConnect
    OnDisconnect = ServerSocketDisconnect
    OnRead = ServerSocketRead
    OnError = SocketError
    Left = 288
    Top = 9
  end
end
