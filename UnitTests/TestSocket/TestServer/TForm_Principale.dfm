object Form_Principale: TForm_Principale
  Left = 373
  Top = 297
  Width = 450
  Height = 314
  Constraints.MinWidth = 450
  Constraints.MinHeight = 340
  Caption = 'Serveur'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxParam: TGroupBox
		Left = 4
		Top = 2
		Width = 432
		Height = 108
		Anchors = [akLeft, akTop, akRight]
		Caption = 'Paramètres connexion'
		TabStop = True
		TabOrder = 0
		object LabelStatus: TLabel
			Left = 12
			Top = 18
			Width = 85
			Height = 13
			Caption = 'Etat :'
		end
		object LabelStatusContent: TLabel
			Left = 108
			Top = 18
			Width = 300
			Height = 13
			Anchors = [akLeft, akTop, akRight]
	    Caption = 'D'#233'sactiv'#233
		end
		object LabelPort: TLabel
			Left = 12
			Top = 44
			Width = 85
			Height = 13
			Caption = 'Port :'
		end
		object EditPort: TEdit
			Left = 108
			Top = 42
			Width = 300
			Height = 21
			Anchors = [akLeft, akTop, akRight]
			TabOrder = 1
			Text = ''
		end
		object Button_Activer: TButton
			Left = 108
			Top = 74
			Width = 85
			Height = 22
			Caption = 'Activer'
			TabOrder = 2
			OnClick = Button_ActiverClick
		end
		object Button_Read: TButton
			Left = 208
			Top = 74
			Width = 85
			Height = 22
			Caption = 'Lecture'
			TabOrder = 3
			OnClick = Button_ReadClick
		end
	end
  object Memo: TMemo
    Left = 4
    Top = 118
    Width = 432
    Height = 166
    TabOrder = 1
		Anchors = [akLeft, akTop, akRight, akBottom]
  end
  object ServerSocket: TServerSocket
    Active = False
    Port = 8080
    ServerType = stNonBlocking
    OnAccept = ServerSocketAccept
    OnConnect = ServerSocketClientConnect
    OnDisconnect = ServerSocketClientDisconnect
    OnRead = ServerSocketClientRead
    OnError = ServerSocketClientError
    Left = 288
    Top = 9
  end
end
