object Form_Principale: TForm_Principale
  Left = 333
  Top = 273
  Width = 450
  Height = 204
  Constraints.MinWidth = 450
  Constraints.MinHeight = 204
  Constraints.MaxHeight = 204
  Caption = 'Client'
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
		Height = 134
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
			Caption = 'D'#233'connect'#233
		end
		object LabelHost: TLabel
			Left = 12
			Top = 44
			Width = 85
			Height = 13
			Caption = 'Host ou IP :'
		end
		object EditHost: TEdit
			Left = 108
			Top = 42
			Width = 300
			Height = 21
			Anchors = [akLeft, akTop, akRight]
			TabOrder = 0
			Text = ''
		end
		object LabelPort: TLabel
			Left = 12
			Top = 70
			Width = 85
			Height = 13
			Caption = 'Port :'
		end
		object EditPort: TEdit
			Left = 108
			Top = 68
			Width = 300
			Height = 21
			Anchors = [akLeft, akTop, akRight]
			TabOrder = 1
			Text = ''
		end
		object Button_Connecter: TButton
			Left = 108
			Top = 100
			Width = 85
			Height = 22
			Caption = 'Connecter'
			TabOrder = 2
			OnClick = Button_ConnecterClick
		end
	end
  object EditText: TEdit
    Left = 12
    Top = 144
    Width = 314
    Height = 21
		Anchors = [akLeft, akTop, akRight]
		Enabled = False
    TabOrder = 1
    Text = 'Test connexion'
  end
  object Button_Envoyer: TButton
    Left = 330
    Top = 142
    Width = 104
    Height = 22
		Anchors = [akTop, akRight]
    Caption = 'Envoyer'
		Enabled = False
    TabOrder = 2
    OnClick = Button_EnvoyerClick
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
    OnError = ClientSocketError
    Left = 402
    Top = 3
  end
end
