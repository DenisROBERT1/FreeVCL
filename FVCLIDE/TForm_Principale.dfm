object Form_Principale: TForm_Principale
	Left = 215
	Top = 171
	Width = 696
	Height = 472
	Caption = 'Environnement de Développement Intégré Free VCL'
	Color = clBtnFace
	Font.Charset = DEFAULT_CHARSET
	Font.Color = clWindowText
	Font.Height = -11
	Font.Name = 'MS Sans Serif'
	Font.Style = []
	Menu = MainMenu1
	OldCreateOrder = False
	Position = poScreenCenter
	OnActivate = FormActivate
	OnClose = FormClose
	PixelsPerInch = 96
	TextHeight = 13
	OnShortCut = Form_PrincipaleShortCut
	object Action_NewWorkSpace: TAction
		OnExecute = Action_NewWorkSpaceExecute
	end
	object Action_OpenWorkSpace: TAction
		OnExecute = Action_OpenWorkSpaceExecute
	end
	object Action_SaveWorkSpace: TAction
		OnExecute = Action_SaveWorkSpaceExecute
	end
	object Action_SaveAsWorkSpace: TAction
		OnExecute = Action_SaveAsWorkSpaceExecute
	end
	object Action_CloseWorkSpace: TAction
		OnExecute = Action_CloseWorkSpaceExecute
	end
	object Action_AddNewFileOnCurrent: TAction
		OnExecute = Action_AddNewFileOnCurrentExecute
	end
	object Action_AddExistingFileOnCurrent: TAction
		OnExecute = Action_AddExistingFileOnCurrentExecute
	end
	object Action_AddNewProjectOnCurrent: TAction
		OnExecute = Action_AddNewProjectOnCurrentExecute
	end
	object Action_AddExistingProjectOnCurrent: TAction
		OnExecute = Action_AddExistingProjectOnCurrentExecute
	end
	object Action_CloseProject: TAction
		OnExecute = Action_CloseProjectExecute
	end
	object Action_ExpandNode: TAction
		OnExecute = Action_ExpandNodeExecute
	end
	object Action_CollapseNode: TAction
		OnExecute = Action_CollapseNodeExecute
	end
	object Action_AddNewFileOnSelected: TAction
		OnExecute = Action_AddNewFileOnSelectedExecute
	end
	object Action_AddExistingFileOnSelected: TAction
		OnExecute = Action_AddExistingFileOnSelectedExecute
	end
	object Action_AddNewProjectOnSelected: TAction
		OnExecute = Action_AddNewProjectOnSelectedExecute
	end
	object Action_AddExistingProjectOnSelected: TAction
		OnExecute = Action_AddExistingProjectOnSelectedExecute
	end
	object Action_RenameProject: TAction
		OnExecute = Action_RenameProjectExecute
	end
	object Action_RemoveProject: TAction
		OnExecute = Action_RemoveProjectExecute
	end
	object Action_EditProject: TAction
		OnExecute = Action_EditProjectExecute
	end
	object Action_EditFile: TAction
		OnExecute = Action_EditFileExecute
	end
	object Action_OpenProjectFile: TAction
		OnExecute = Action_OpenProjectFileExecute
	end
	object Action_NewTextFile: TAction
		OnExecute = Action_NewTextFileExecute
	end
	object Action_NewBinFile: TAction
		OnExecute = Action_NewBinFileExecute
	end
	object Action_OpenTextFile: TAction
		OnExecute = Action_OpenTextFileExecute
	end
	object Action_OpenBinFile: TAction
		OnExecute = Action_OpenBinFileExecute
	end
	object Action_SaveFile: TAction
		OnExecute = Action_SaveFileExecute
	end
	object Action_SaveAllFiles: TAction
		OnExecute = Action_SaveAllFilesExecute
	end
	object Action_SaveAsFile: TAction
		OnExecute = Action_SaveAsFileExecute
	end
	object Action_CopyPath: TAction
		OnExecute = Action_CopyPathExecute
	end
	object Action_OpenFolder: TAction
		OnExecute = Action_OpenFolderExecute
	end
	object Action_OpenFichierAssocie1: TAction
		OnExecute = Action_OpenFichierAssocie1Execute
	end
	object Action_OpenFichierAssocie2: TAction
		OnExecute = Action_OpenFichierAssocie2Execute
	end
	object Action_NewDialog: TAction
		OnExecute = Action_NewDialogExecute
	end
	object Action_OpenDialog: TAction
		OnExecute = Action_OpenDialogExecute
	end
	object Action_SaveDialog: TAction
		OnExecute = Action_SaveDialogExecute
	end
	object Action_SaveAsDialog: TAction
		OnExecute = Action_SaveAsDialogExecute
	end
	object Action_NewBmp: TAction
		OnExecute = Action_NewBmpExecute
	end
	object Action_NewIcon: TAction
		OnExecute = Action_NewIconExecute
	end
	object Action_NewCursor: TAction
		OnExecute = Action_NewCursorExecute
	end
	object Action_OpenGraphic: TAction
		OnExecute = Action_OpenGraphicExecute
	end
	object Action_SaveGraphic: TAction
		OnExecute = Action_SaveGraphicExecute
	end
	object Action_SaveAsGraphic: TAction
		OnExecute = Action_SaveAsGraphicExecute
	end
	object Action_Close: TAction
		OnExecute = Action_CloseExecute
	end
	object Action_CloseAll: TAction
		OnExecute = Action_CloseAllExecute
	end
	object Action_CloseOthers: TAction
		OnExecute = Action_CloseOthersExecute
	end
	object Action_Print: TAction
		OnExecute = Action_PrintExecute
	end
	object Action_Quit: TAction
		OnExecute = Action_QuitExecute
	end
	object Action_Undo: TAction
		OnExecute = Action_UndoExecute
	end
	object Action_Redo: TAction
		OnExecute = Action_RedoExecute
	end
	object Action_CutToClipboard: TAction
		OnExecute = Action_CutToClipboardExecute
	end
	object Action_CopyToClipboard: TAction
		OnExecute = Action_CopyToClipboardExecute
	end
	object Action_PasteFromClipboard: TAction
		OnExecute = Action_PasteFromClipboardExecute
	end
	object Action_RefPrevious: TAction
		OnExecute = Action_RefPreviousExecute
	end
	object Action_RefNext: TAction
		OnExecute = Action_RefNextExecute
	end
	object Action_SelectAll: TAction
		OnExecute = Action_SelectAllExecute
	end
	object Action_Goto: TAction
		OnExecute = Action_GotoExecute
	end
	object Action_LowerCase: TAction
		OnExecute = Action_LowerCaseExecute
	end
	object Action_UpperCase: TAction
		OnExecute = Action_UpperCaseExecute
	end
	object Action_Comment: TAction
		OnExecute = Action_CommentExecute
	end
	object Action_Uncomment: TAction
		OnExecute = Action_UncommentExecute
	end
	object Action_AddBookmark: TAction
		OnExecute = Action_AddBookmarkExecute
	end
	object Action_DelBookmark: TAction
		OnExecute = Action_DelBookmarkExecute
	end
	object Action_ClearBookmarks: TAction
		OnExecute = Action_ClearBookmarksExecute
	end
	object Action_FileFormat: TAction
		OnExecute = Action_FileFormatExecute
	end
	object Action_TabsToSpaces: TAction
		OnExecute = Action_TabsToSpacesExecute
	end
	object Action_SpacesToTabs: TAction
		OnExecute = Action_SpacesToTabsExecute
	end
	object Action_SpacesToTabsBegin: TAction
		OnExecute = Action_SpacesToTabsBeginExecute
	end
	object Action_EndingSpaces: TAction
		OnExecute = Action_EndingSpacesExecute
	end
	object Action_Find: TAction
		OnExecute = Action_FindExecute
	end
	object Action_FindInFiles: TAction
		OnExecute = Action_FindInFilesExecute
	end
	object Action_Replace: TAction
		OnExecute = Action_ReplaceExecute
	end
	object Action_ReplaceInFiles: TAction
		OnExecute = Action_ReplaceInFilesExecute
	end
	object Action_FindPrevious: TAction
		OnExecute = Action_FindPreviousExecute
	end
	object Action_FindNext: TAction
		OnExecute = Action_FindNextExecute
	end
	object Action_FindSelPrevious: TAction
		OnExecute = Action_FindSelPreviousExecute
	end
	object Action_FindSelNext: TAction
		OnExecute = Action_FindSelNextExecute
	end
	object Action_RecMacro: TAction
		OnExecute = Action_RecMacroExecute
	end
	object Action_PlayMacro: TAction
		OnExecute = Action_PlayMacroExecute
	end
	object Action_RepeatMacro: TAction
		OnExecute = Action_RepeatMacroExecute
	end
	object Action_Models: TAction
		OnExecute = Action_ModelsExecute
	end
	object Action_PreCompilCurrent: TAction
		OnExecute = Action_PreCompilCurrentExecute
	end
	object Action_BuildCurrent: TAction
		OnExecute = Action_BuildCurrentExecute
	end
	object Action_RebuildCurrent: TAction
		OnExecute = Action_RebuildCurrentExecute
	end
	object Action_PreCompilSelected: TAction
		OnExecute = Action_PreCompilSelectedExecute
	end
	object Action_BuildSelected: TAction
		OnExecute = Action_BuildSelectedExecute
	end
	object Action_RebuildSelected: TAction
		OnExecute = Action_RebuildSelectedExecute
	end
	object Action_PreCompilAll: TAction
		OnExecute = Action_PreCompilAllExecute
	end
	object Action_BuildAll: TAction
		OnExecute = Action_BuildAllExecute
	end
	object Action_RebuildAll: TAction
		OnExecute = Action_RebuildAllExecute
	end
	object Action_SetCurrent: TAction
		OnExecute = Action_SetCurrentExecute
	end
	object Action_ViewOutput: TAction
		OnExecute = Action_ViewOutputExecute
	end
	object Action_Properties: TAction
		OnExecute = Action_PropertiesExecute
	end
	object Action_RunCurrent: TAction
		OnExecute = Action_RunCurrentExecute
	end
	object Action_DebugCurrent: TAction
		OnExecute = Action_DebugCurrentExecute
	end
	object Action_StopDebug: TAction
		OnExecute = Action_StopDebugExecute
	end
	object Action_StepByStepNext: TAction
		OnExecute = Action_StepByStepNextExecute
	end
	object Action_StepByStepIn: TAction
		OnExecute = Action_StepByStepInExecute
	end
	object Action_StepByStepOut: TAction
		OnExecute = Action_StepByStepOutExecute
	end
	object Action_AddBreakpoint: TAction
		OnExecute = Action_AddBreakpointExecute
	end
	object Action_DelBreakpoint: TAction
		OnExecute = Action_DelBreakpointExecute
	end
	object Action_ClearBreakpoints: TAction
		OnExecute = Action_ClearBreakpointsExecute
	end
	object Action_Statistics: TAction
		OnExecute = Action_StatisticsExecute
	end
	object Action_CodeFormat: TAction
		OnExecute = Action_CodeFormatExecute
	end
	object Action_ConvertNumbers: TAction
		OnExecute = Action_ConvertNumbersExecute
	end
	object Action_ConfigTools: TAction
		OnExecute = Action_ConfigToolsExecute
	end
	object Action_Language: TAction
		OnExecute = Action_LanguageExecute
	end
	object Action_CodeOnly: TAction
		OnExecute = Action_CodeOnlyExecute
	end
	object Action_EdiOptionsGenerales: TAction
		OnExecute = Action_EdiOptionsGeneralesExecute
	end
	object Action_ShortCuts: TAction
		OnExecute = Action_ShortCutsExecute
	end
	object Action_Colors: TAction
		OnExecute = Action_ColorsExecute
	end
	object Action_Font: TAction
		OnExecute = Action_FontExecute
	end
	object Action_CodeFormatParams: TAction
		OnExecute = Action_CodeFormatParamsExecute
	end
	object Action_DefaultResponses: TAction
		OnExecute = Action_DefaultResponsesExecute
	end
	object Action_FVCLIDEHelp: TAction
		OnExecute = Action_FVCLIDEHelpExecute
	end
	object Action_FreeVCLHelp: TAction
		OnExecute = Action_FreeVCLHelpExecute
	end
	object Action_Autocompletion: TAction
		OnExecute = Action_AutocompletionExecute
	end
	object Action_APropos: TAction
		OnExecute = Action_AProposExecute
	end
	object Action_ContactMe: TAction
		OnExecute = Action_ContactMeExecute
	end
	object Action_MiseAJour: TAction
		OnExecute = Action_MiseAJourExecute
	end
	object Action_LogCopyLine: TAction
		OnExecute = Action_LogCopyLineExecute
	end
	object Action_LogCopyAll: TAction
		OnExecute = Action_LogCopyAllExecute
	end
	object MainMenu: TMainMenu
		Left = 656
		Top = 32
		object MenuItem_FilesAndProjects: TMenuItem
			Caption = '&Fichiers'
			object MenuItem_WorkSpaces: TMenuItem
				Caption = '&Espaces de travail'
				object MenuItem_NewWorkSpace: TMenuItem
					Caption = '&Nouvel espace de travail'
					Action = Action_NewWorkSpace
				end
				object MenuItem_OpenWorkSpace: TMenuItem
					Caption = '&Ouvrir un espace de travail'
					Action = Action_OpenWorkSpace
				end
				object MenuItem_SaveWorkSpace: TMenuItem
					Caption = '&Enregistrer l'#39'espace de travail'
					Action = Action_SaveWorkSpace
				end
				object MenuItem_SaveAsWorkSpace: TMenuItem
					Caption = 'Enregistrer l'#39'espace de travail &sous ...'
					Action = Action_SaveAsWorkSpace
				end
				object MenuItem_CloseWorkSpace: TMenuItem
					Caption = '&Fermer'
					Action = Action_CloseWorkSpace
				end
				object MenuItem_Separator1: TMenuItem
					Caption = '-'
				end
			end
			object MenuItem_Projects: TMenuItem
				Caption = '&Projets'
				object MenuItem_NewProject: TMenuItem
					Caption = '&Nouveau projet'
					Action = Action_AddNewProjectOnCurrent
				end
				object MenuItem_OpenProject: TMenuItem
					Caption = '&Ouvrir un projet'
					Action = Action_AddExistingProjectOnCurrent
				end
				object MenuItem_CloseProject: TMenuItem
					Caption = '&Fermer'
					Action = Action_CloseProject
				end
				object MenuItem_Separator2: TMenuItem
					Caption = '-'
				end
			end
			object MenuItem_Files: TMenuItem
				Caption = '&Fichiers'
				object MenuItem_NewFile: TMenuItem
					Caption = '&Nouveau fichier'
					object MenuItem_NewTextFile: TMenuItem
						Caption = 'Nouveau fichier &texte'
						Action = Action_NewTextFile
					end
					object MenuItem_NewBinFile: TMenuItem
						Caption = 'Nouveau fichier &binaire'
						Action = Action_NewBinFile
					end
				end
				object MenuItem_OpenFile: TMenuItem
					Caption = '&Ouvrir un fichier ...'
					object MenuItem_OpenTextFile: TMenuItem
						Caption = 'Ouvrir un fichier &texte'
						Action = Action_OpenTextFile
					end
					object MenuItem_OpenBinFile: TMenuItem
						Caption = 'Ouvrir un fichier &binaire'
						Action = Action_OpenBinFile
					end
				end
				object MenuItem_SaveFile: TMenuItem
					Caption = '&Enregistrer le fichier'
					Action = Action_SaveFile
				end
				object MenuItem_SaveAllFiles: TMenuItem
					Caption = 'Enregistrer &tous les fichiers'
					Action = Action_SaveAllFiles
				end
				object MenuItem_SaveAsFile: TMenuItem
					Caption = 'Enregistrer le fichier &sous ...'
					Action = Action_SaveAsFile
				end
				object MenuItem_CloseFile: TMenuItem
					Caption = '&Fermer'
					Action = Action_Close
				end
				object MenuItem_Separator3: TMenuItem
					Caption = '-'
				end
			end
			object MenuItem_Dialogs: TMenuItem
				Caption = '&Dialogues'
				object MenuItem_NewDialog: TMenuItem
					Caption = '&Nouveau dialogue'
					Action = Action_NewDialog
				end
				object MenuItem_OpenDialog: TMenuItem
					Caption = '&Ouvrir un dialogue'
					Action = Action_OpenDialog
				end
				object MenuItem_SaveDialog: TMenuItem
					Caption = '&Enregistrer le dialogue'
					Action = Action_SaveDialog
				end
				object MenuItem_SaveAsDialog: TMenuItem
					Caption = 'Enregistrer le dialogue &sous ...'
					Action = Action_SaveAsDialog
				end
				object MenuItem_CloseDialog: TMenuItem
					Caption = '&Fermer'
					Action = Action_Close
				end
				object MenuItem_Separator4: TMenuItem
					Caption = '-'
				end
			end
			object MenuItem_Graphics: TMenuItem
				Caption = '&Graphiques'
				object MenuItem_NewGraphic: TMenuItem
					Caption = '&Nouveau graphique'
					object MenuItem_NewBmp: TMenuItem
						Caption = 'Nouveau fichier &BMP'
						Action = Action_NewBmp
					end
					object MenuItem_NewIcon: TMenuItem
						Caption = 'Nouveau fichier &ICO'
						Action = Action_NewIcon
					end
					object MenuItem_NewCursor: TMenuItem
						Caption = 'Nouveau fichier &CUR'
						Action = Action_NewCursor
					end
				end
				object MenuItem_OpenGraphic: TMenuItem
					Caption = '&Ouvrir un graphique (Image, icône, curseur)'
					Action = Action_OpenGraphic
				end
				object MenuItem_SaveFile: TMenuItem
					Caption = '&Enregistrer le graphique'
					Action = Action_SaveGraphic
				end
				object MenuItem_SaveAsGraphic: TMenuItem
					Caption = 'Enregistrer le graphique &sous ...'
					Action = Action_SaveAsGraphic
				end
				object MenuItem_CloseGraphic: TMenuItem
					Caption = '&Fermer'
					Action = Action_Close
				end
				object MenuItem_Separator5: TMenuItem
					Caption = '-'
				end
			end
			object MenuItem_Separator6: TMenuItem
				Caption = '-'
			end
			object MenuItem_Close: TMenuItem
				Caption = '&Fermer le fichier sélectionné'
				Action = Action_Close
			end
			object MenuItem_CloseAll: TMenuItem
				Caption = '&Tout fermer'
				Action = Action_CloseAll
			end
			object MenuItem_CloseOthers: TMenuItem
				Caption = 'Fermer tous les &autres'
				Action = Action_CloseOthers
			end
			object MenuItem_Separator7: TMenuItem
				Caption = '-'
			end
			object MenuItem_Print: TMenuItem
				Caption = 'Im&primer'
				Action = Action_Print
			end
			object MenuItem_Separator8: TMenuItem
				Caption = '-'
			end
			object MenuItem_Quit: TMenuItem
				Caption = '&Quitter'
				Action = Action_Quit
			end
		end
		object MenuItem_Edit: TMenuItem
			Caption = 'Edition'
			object MenuItem_Undo: TMenuItem
				Caption = 'Défaire'
				Action = Action_Undo
			end
			object MenuItem_Redo: TMenuItem
				Caption = 'Refaire'
				Action = Action_Redo
			end
			object MenuItem_Separator9: TMenuItem
				Caption = '-'
			end
			object MenuItem_CutToClipboard: TMenuItem
				Caption = 'Couper'
				Action = Action_CutToClipboard
			end
			object MenuItem_CopyToClipboard: TMenuItem
				Caption = 'Copier'
				Action = Action_CopyToClipboard
			end
			object MenuItem_PasteFromClipboard: TMenuItem
				Caption = 'Coller'
				Action = Action_PasteFromClipboard
			end
			object MenuItem_Separator10: TMenuItem
				Caption = '-'
			end
			object MenuItem_RefPrevious: TMenuItem
				Caption = 'Référence précédente'
				Action = Action_RefPrevious
			end
			object MenuItem_RefNext: TMenuItem
				Caption = 'Référence suivante'
				Action = Action_RefNext
			end
			object MenuItem_Separator11: TMenuItem
				Caption = '-'
			end
			object MenuItem_SelectAll: TMenuItem
				Caption = 'Tout sélectionner'
				Action = Action_SelectAll
			end
			object MenuItem_Goto: TMenuItem
				Caption = 'Aller à la ligne numéro ...'
				Action = Action_Goto
			end
			object MenuItem_Separator12: TMenuItem
				Caption = '-'
			end
			object MenuItem_Advanced: TMenuItem
				Caption = 'Fonctions avancées'
				object MenuItem_LowerCase: TMenuItem
					Caption = 'Mettre en minuscules'
					Action = Action_LowerCase
				end
				object MenuItem_UpperCase: TMenuItem
					Caption = 'Mettre en majuscules'
					Action = Action_UpperCase
				end
				object MenuItem_Separator13: TMenuItem
					Caption = '-'
				end
				object MenuItem_Comment: TMenuItem
					Caption = 'Commenter'
					Action = Action_Comment
				end
				object MenuItem_Uncomment: TMenuItem
					Caption = 'Décommenter'
					Action = Action_Uncomment
				end
			end
			object MenuItem_Bookmarks: TMenuItem
				Caption = 'Marque-pages'
				object MenuItem_AddBookmark: TMenuItem
					Caption = 'Ajouter un marque-page'
					Action = Action_AddBookmark
				end
				object MenuItem_DelBookmark: TMenuItem
					Caption = 'Supprimer le marque-page'
					Action = Action_DelBookmark
				end
				object MenuItem_ClearBookmarks: TMenuItem
					Caption = 'Effacer tous les marque-pages'
					Action = Action_ClearBookmarks
				end
			end
			object MenuItem_Document: TMenuItem
				Caption = 'Document'
				object MenuItem_FileFormat: TMenuItem
					Caption = 'Format de fichier'
					Action = Action_FileFormat
				end
				object MenuItem_TabsToSpaces: TMenuItem
					Caption = 'Tabulations -> espaces'
					Action = Action_TabsToSpaces
				end
				object MenuItem_SpacesToTabs: TMenuItem
					Caption = 'Espaces -> tabulations'
					Action = Action_SpacesToTabs
				end
				object MenuItem_SpacesToTabsBegin: TMenuItem
					Caption = 'Espaces -> tabulations (en début de ligne)'
					Action = Action_SpacesToTabsBegin
				end
				object MenuItem_EndingSpaces: TMenuItem
					Caption = 'Supprimer les espaces de fin'
					Action = Action_EndingSpaces
				end
			end
		end
		object MenuItem_FindAndReplace: TMenuItem
			Caption = 'Recherche'
			object MenuItem_Find: TMenuItem
				Caption = 'Chercher'
				Action = Action_Find
			end
			object MenuItem_FindInFiles: TMenuItem
				Caption = 'Chercher dans des fichiers'
				Action = Action_FindInFiles
			end
			object MenuItem_Separator14: TMenuItem
				Caption = '-'
			end
			object MenuItem_Replace: TMenuItem
				Caption = 'Remplacer'
				Action = Action_Replace
			end
			object MenuItem_ReplaceInFiles: TMenuItem
				Caption = 'Remplacer dans des fichiers'
				Action = Action_ReplaceInFiles
			end
			object MenuItem_Separator15: TMenuItem
				Caption = '-'
			end
			object MenuItem_FindPrevious: TMenuItem
				Caption = 'Chercher précédent'
				Action = Action_FindPrevious
			end
			object MenuItem_FindNext: TMenuItem
				Caption = 'Chercher suivant'
				Action = Action_FindNext
			end
			object MenuItem_FindSelPrevious: TMenuItem
				Caption = 'Chercher texte sélectionné précédent'
				Action = Action_FindSelPrevious
			end
			object MenuItem_FindSelNext: TMenuItem
				Caption = 'Chercher texte sélectionné suivant'
				Action = Action_FindSelNext
			end
		end
		object MenuItem_Macros: TMenuItem
			Caption = 'Macros'
			object MenuItem_RecMacro: TMenuItem
				Caption = 'Enregistrer une macro'
				Action = Action_RecMacro
			end
			object MenuItem_PlayMacro: TMenuItem
				Caption = 'Exécuter une macro'
				Action = Action_PlayMacro
			end
			object MenuItem_RepeatMacro: TMenuItem
				Caption = 'Répéter une macro...'
				Action = Action_RepeatMacro
			end
			object MenuItem_Separator16: TMenuItem
				Caption = '-'
			end
			object MenuItem_Models: TMenuItem
				Caption = 'Modèles de code'
				Action = Action_Models
			end
		end
		object MenuItem_BuildAndView: TMenuItem
			Caption = 'Construire'
			object MenuItem_PreCompilCurrent: TMenuItem
				Caption = '&Pré-compiler le projet courant'
				Action = Action_PreCompilCurrent
			end
			object MenuItem_BuildCurrent: TMenuItem
				Caption = '&Construire le projet courant'
				Action = Action_BuildCurrent
			end
			object MenuItem_RebuildCurrent: TMenuItem
				Caption = '&Reconstruire le projet courant'
				Action = Action_RebuildCurrent
			end
			object MenuItem_Separator17: TMenuItem
				Caption = '-'
			end
			object MenuItem_PreCompilAll: TMenuItem
				Caption = '&Pré-compiler tous les projets'
				Action = Action_PreCompilAll
			end
			object MenuItem_BuildAll: TMenuItem
				Caption = '&Construire tous les projets'
				Action = Action_BuildAll
			end
			object MenuItem_RebuildAll: TMenuItem
				Caption = 'Reconstruire &tous les projets'
				Action = Action_RebuildAll
			end
			object MenuItem_Separator18: TMenuItem
				Caption = '-'
			end
			object MenuItem_SetCurrent: TMenuItem
				Caption = 'Définir sélectionné comme projet &courant'
				Action = Action_SetCurrent
			end
			object MenuItem_ViewOutput: TMenuItem
				Caption = '&Voir le fichier de sortie'
				Action = Action_ViewOutput
			end
		end
		object MenuItem_DebugAndRun: TMenuItem
			Caption = 'Déboguer'
			object MenuItem_RunCurrent: TMenuItem
				Caption = '&Exécuter'
				Action = Action_RunCurrent
			end
			object MenuItem_DebugCurrent: TMenuItem
				Caption = '&Lancer le débogueur'
				Action = Action_DebugCurrent
			end
			object MenuItem_StopDebug: TMenuItem
				Caption = '&Arrêter le débogage'
				Enabled = False
				Action = Action_StopDebug
			end
			object MenuItem_Separator19: TMenuItem
				Caption = '-'
			end
			object MenuItem_StepByStepNext: TMenuItem
				Caption = '&Pas à pas (instruction suivante)'
				Action = Action_StepByStepNext
			end
			object MenuItem_StepByStepIn: TMenuItem
				Caption = 'Pas à pas (&entrer dans la fonction)'
				Action = Action_StepByStepIn
			end
			object MenuItem_StepByStepOut: TMenuItem
				Caption = 'Pas à pas (&sortir de la fonction)'
				Action = Action_StepByStepOut
			end
			object MenuItem_Separator20: TMenuItem
				Caption = '-'
			end
			object MenuItem_Breakpoints: TMenuItem
				Caption = 'Points d'#39'arrêt'
				object MenuItem_AddBreakpoint: TMenuItem
					Caption = 'Ajouter un point d'#39'arrêt'
					Action = Action_AddBreakpoint
				end
				object MenuItem_DelBreakpoint: TMenuItem
					Caption = 'Supprimer le point d'#39'arrêt'
					Action = Action_DelBreakpoint
				end
				object MenuItem_ClearBreakpoints: TMenuItem
					Caption = 'Effacer tous les points d'#39'arrêt'
					Action = Action_ClearBreakpoints
				end
			end
		end
		object MenuItem_Tools: TMenuItem
			Caption = 'Outils'
			object MenuItem_Statistics: TMenuItem
				Caption = 'Statistiques'
				Action = Action_Statistics
			end
			object MenuItem_CodeFormat: TMenuItem
				Caption = 'Formatage du code'
				Action = Action_CodeFormat
			end
			object MenuItem_ConvertNumbers: TMenuItem
				Caption = 'Conversion de nombres'
				Action = Action_ConvertNumbers
			end
			object MenuItem_Separator21: TMenuItem
				Caption = '-'
			end
			object MenuItem_UserTools: TMenuItem
				Caption = 'Outils utilisateur'
				object MenuItem_ConfigTools: TMenuItem
					Caption = 'Configurer les outils'
					Action = Action_ConfigTools
				end
				object MenuItem_Separator22: TMenuItem
					Caption = '-'
				end
			end
		end
		object MenuItem_Options: TMenuItem
			Caption = 'Options'
			object MenuItem_Language: TMenuItem
				Caption = '&Langue'
				Action = Action_Language
			end
			object MenuItem_CodeOnly: TMenuItem
				Caption = 'Voir le &code uniquement'
				Action = Action_CodeOnly
			end
			object MenuItem_Separator23: TMenuItem
				Caption = '-'
			end
			object MenuItem_EdiOptionsGenerales: TMenuItem
				Caption = 'Options générales'
				Action = Action_EdiOptionsGenerales
			end
			object MenuItem_ShortCuts: TMenuItem
				Caption = 'Raccourcis clavier'
				Action = Action_ShortCuts
			end
			object MenuItem_Colors: TMenuItem
				Caption = 'Coloration syntaxique'
				Action = Action_Colors
			end
			object MenuItem_Font: TMenuItem
				Caption = 'Police de caractères'
				Action = Action_Font
			end
			object MenuItem_CodeFormatParams: TMenuItem
				Caption = 'Paramètres de formatage'
				Action = Action_CodeFormatParams
			end
			object MenuItem_DefaultResponses: TMenuItem
				Caption = 'Réponses par défaut'
				Action = Action_DefaultResponses
			end
		end
		object MenuItem_Help: TMenuItem
			Caption = 'Aide'
			RightJustify = True
			object MenuItem_FVCLIDEHelp: TMenuItem
				Caption = 'Aide Free VCL'
				Action = Action_FVCLIDEHelp
			end
			object MenuItem_FreeVCLHelp: TMenuItem
				Caption = 'Aide en ligne (syntaxe)'
				Action = Action_FreeVCLHelp
			end
			object MenuItem_Autocompletion: TMenuItem
				Caption = 'Auto-complétion'
				Action = Action_Autocompletion
			end
			object MenuItem_APropos: TMenuItem
				Caption = 'À propos'
				Action = Action_APropos
			end
			object MenuItem_ContactMe: TMenuItem
				Caption = 'Me contacter'
				Action = Action_ContactMe
			end
			object MenuItem_MiseAJour: TMenuItem
				Caption = 'Mise à jour'
				Action = Action_MiseAJour
			end
		end
	end
	object PopupMenu_Projects: TPopupMenu
		Left = 624
		Top = 32
		OnPopup = PopupMenu_ProjectsPopup
		object MenuItemPopup_ExpandNode: TMenuItem
			Caption = 'Tout Dé&velopper'
			Action = Action_ExpandNode
		end
		object MenuItemPopup_CollapseNode: TMenuItem
			Caption = 'Tout &Réduire'
			Action = Action_CollapseNode
		end
		object MenuItem_Separator24: TMenuItem
			Caption = '-'
		end
		object MenuItemPopup_AddNewFileOnSelected: TMenuItem
			Caption = 'Ajouter un &nouveau fichier au projet'
			Action = Action_AddNewFileOnSelected
		end
		object MenuItemPopup_AddExistingFileOnSelected: TMenuItem
			Caption = '&Ajouter des fichiers existants au projet'
			Action = Action_AddExistingFileOnSelected
		end
		object MenuItem_Separator25: TMenuItem
			Caption = '-'
		end
		object MenuItemPopup_AddNewProjectOnSelected: TMenuItem
			Caption = 'Ajouter un &nouveau projet'
			Action = Action_AddNewProjectOnSelected
		end
		object MenuItemPopup_AddExistingProjectOnSelected: TMenuItem
			Caption = '&Ajouter un projet existant'
			Action = Action_AddExistingProjectOnSelected
		end
		object MenuItemPopup_RenameProject: TMenuItem
			Caption = '&Renommer'
			Action = Action_RenameProject
		end
		object MenuItemPopup_RemoveProject: TMenuItem
			Caption = '&Enlever'
			Action = Action_RemoveProject
		end
		object MenuItem_Separator26: TMenuItem
			Caption = '-'
		end
		object MenuItemPopup_EditProject: TMenuItem
			Caption = '&Modifier'
			Action = Action_EditProject
		end
		object MenuItemPopup_EditFile: TMenuItem
			Caption = '&Editer le fichier'
			Action = Action_EditFile
		end
		object MenuItemPopup_OpenProjectFile: TMenuItem
			Caption = '&Ouvrir avec le programme associé'
			Action = Action_OpenProjectFile
		end
		object MenuItem_Separator27: TMenuItem
			Caption = '-'
		end
		object MenuItemPopup_PreCompilSelected: TMenuItem
			Caption = '&Pré-compiler ce projet'
			Action = Action_PreCompilSelected
		end
		object MenuItemPopup_BuildSelected: TMenuItem
			Caption = '&Construire ce projet'
			Action = Action_BuildSelected
		end
		object MenuItemPopup_RebuildSelected: TMenuItem
			Caption = '&Reconstruire ce projet'
			Action = Action_RebuildSelected
		end
		object MenuItem_Separator28: TMenuItem
			Caption = '-'
		end
		object MenuItemPopup_SetCurrent: TMenuItem
			Caption = 'Définir comme projet &courant'
			Action = Action_SetCurrent
		end
		object MenuItemPopup_ViewOutput: TMenuItem
			Caption = '&Voir le fichier de sortie'
			Action = Action_ViewOutput
		end
		object MenuItem_Separator29: TMenuItem
			Caption = '-'
		end
		object MenuItemPopup_Properties: TMenuItem
			Caption = '&Propriétés'
			Action = Action_Properties
		end
	end
	object PopupMenu_Files: TPopupMenu
		Left = 624
		Top = 32
		OnPopup = PopupMenu_FilesPopup
		object MenuItemPopup_Close: TMenuItem
			Caption = '&Fermer'
			Action = Action_Close
		end
		object MenuItemPopup_CloseAll: TMenuItem
			Caption = '&Tout fermer'
			Action = Action_CloseAll
		end
		object MenuItemPopup_CloseOthers: TMenuItem
			Caption = 'Fermer tous les &autres'
			Action = Action_CloseOthers
		end
		object MenuItem_Separator30: TMenuItem
			Caption = '-'
		end
		object MenuItemPopup_CopyPath: TMenuItem
			Caption = 'Copier le nom complet'
			Action = Action_CopyPath
		end
		object MenuItemPopup_OpenFolder: TMenuItem
			Caption = 'Ouvrir le répertoire du fichier'
			Action = Action_OpenFolder
		end
		object MenuItemPopup_OpenFichierAssocie1: TMenuItem
			Caption = 'Ouvrir le fichier .h'
			Action = Action_OpenFichierAssocie1
		end
		object MenuItemPopup_OpenFichierAssocie2: TMenuItem
			Caption = 'Ouvrir le fichier .dfm'
			Action = Action_OpenFichierAssocie2
		end
	end
	object ImageList_Buttons: TImageList
		Left = 564
		Width = 16
		Height = 16
		ResourceName = 'BMP_IMAGELISTBUTTONS'
	end
	object ToolBar: TToolBar
		Left = 0
		Top = 0
		Width = 688
		Height = 25
		Flat = True
		Images = ImageList_Buttons
		object ToolButton_NewTextFile: TToolButton
			Left = 23
			Top = 0
			Hint = 'Nouveau'
			ImageIndex = 0
			ParentShowHint = False
			ShowHint = True
			Action = Action_NewTextFile
		end
		object ToolButton_OpenTextFile: TToolButton
			Left = 46
			Top = 0
			Hint = 'Ouvrir'
			ImageIndex = 1
			ParentShowHint = False
			ShowHint = True
			Action = Action_OpenTextFile
		end
		object ToolButton_SaveFile: TToolButton
			Left = 69
			Top = 0
			Hint = 'Enregistrer'
			ImageIndex = 2
			ParentShowHint = False
			ShowHint = True
			Action = Action_SaveFile
		end
		object ToolButton_SaveAllFiles: TToolButton
			Left = 92
			Top = 0
			Hint = 'Enregistrer tout'
			ImageIndex = 3
			ParentShowHint = False
			ShowHint = True
			Action = Action_SaveAllFiles
		end
		object ToolButton_SaveAsFile: TToolButton
			Left = 115
			Top = 0
			Hint = 'Enregistrer sous'
			ImageIndex = 4
			ParentShowHint = False
			ShowHint = True
			Action = Action_SaveAsFile
		end
		object ToolButton_Separator1: TToolButton
			Left = 138
			Top = 0
			Width = 8
			Style = tbsSeparator
		end
		object ToolButton_BuildCurrent: TToolButton
			Left = 146
			Top = 0
			Hint = 'Compiler'
			ImageIndex = 5
			ParentShowHint = False
			ShowHint = True
			Action = Action_BuildCurrent
		end
		object ToolButton_RunCurrent: TToolButton
			Left = 154
			Top = 0
			Hint = 'Exécuter'
			ImageIndex = 6
			ParentShowHint = False
			ShowHint = True
			Action = Action_RunCurrent
		end
		object ToolButton_DebugCurrent: TToolButton
			Left = 162
			Top = 0
			Hint = 'Déboguer'
			ImageIndex = 7
			ParentShowHint = False
			ShowHint = True
			Action = Action_DebugCurrent
		end
		object ToolButton_Separator2: TToolButton
			Left = 170
			Top = 0
			Width = 8
			Style = tbsSeparator
		end
		object ToolButton_CodeOnly: TToolButton
			Left = 178
			Top = 0
			Hint = 'Voir le code uniquement'
			ImageIndex = 8
			ParentShowHint = False
			ShowHint = True
			Action = Action_CodeOnly
		end
	end
	object StatusBar: TStatusBar
		Left = 0
		Top = 407
		Width = 688
		Height = 19
		Panels = <100, -1>
		SimplePanel = False
	end
	object PopupMenu_Log: TPopupMenu
		Left = 624
		Top = 32
		object MenuItemPopup_LogCopyLine: TMenuItem
			Caption = '&Copier la ligne'
			Action = Action_LogCopyLine
		end
		object MenuItemPopup_LogCopyAll: TMenuItem
			Caption = '&Tout Copier'
			Action = Action_LogCopyAll
		end
	end
	object PageControl_Logs: TPageControl
		Left = 0
		Top = 0
		Width = 688
		Height = 100
		Align = alBottom
		TabIndex = 0
		TabOrder = 1
		object TabSheet_Search: TTabSheet
			Left = 0
			Top = 0
			Width = 688
			Height = 76
			Caption = 'Recherche'
			object ListBox_Search: TListBox
				Left = 0
				Top = 0
				Width = 680
				Height = 72
				Align = alClient
				PopupMenu = PopupMenu_Log
				OnContextPopup = ListBox_LogContextPopup
				OnDblClick = ListBox_SearchDblClick
			end
		end
		object TabSheet_Bookmarks: TTabSheet
			Left = 0
			Top = 0
			Width = 688
			Height = 76
			Caption = 'Marque-pages'
			object ListBox_Bookmarks: TListBox
				Left = 0
				Top = 0
				Width = 680
				Height = 72
				Align = alClient
				PopupMenu = PopupMenu_Log
				OnContextPopup = ListBox_LogContextPopup
				OnDblClick = ListBox_BookmarksDblClick
			end
		end
		object TabSheet_Build: TTabSheet
			Left = 0
			Top = 0
			Width = 688
			Height = 76
			Caption = 'Construction'
			object ListBox_Build: TListBox
				Left = 0
				Top = 0
				Width = 680
				Height = 72
				Align = alClient
				PopupMenu = PopupMenu_Log
				OnContextPopup = ListBox_LogContextPopup
				OnDblClick = ListBox_BuildDblClick
			end
		end
		object TabSheet_Process: TTabSheet
			Left = 0
			Top = 0
			Width = 688
			Height = 76
			Caption = 'Traitements'
			object ListBox_Process: TListBox
				Left = 0
				Top = 0
				Width = 680
				Height = 72
				Align = alClient
				PopupMenu = PopupMenu_Log
				OnContextPopup = ListBox_LogContextPopup
				OnDblClick = ListBox_ProcessDblClick
			end
		end
		object TabSheet_Breakpoints: TTabSheet
			Left = 0
			Top = 0
			Width = 688
			Height = 76
			Caption = 'Points d'#39'arrêt'
			object ListBox_Breakpoints: TListBox
				Left = 0
				Top = 0
				Width = 680
				Height = 72
				Align = alClient
				PopupMenu = PopupMenu_Log
				OnContextPopup = ListBox_LogContextPopup
				OnDblClick = ListBox_BreakpointsDblClick
			end
		end
		object TabSheet_CallStack: TTabSheet
			Left = 0
			Top = 0
			Width = 688
			Height = 76
			Caption = 'Pile d'#39'appel'
			object ListBox_CallStack: TListBox
				Left = 0
				Top = 0
				Width = 680
				Height = 72
				Align = alClient
				PopupMenu = PopupMenu_Log
				OnContextPopup = ListBox_LogContextPopup
				OnDblClick = ListBox_CallStackDblClick
			end
		end
	end
	object Splitter1: TSplitter
		Left = 169
		Top = 25
		Width = 516
		Height = 5
		Cursor = crVSplit
		Align = alBottom
	end
	object ImageList_TreeStates: TImageList
		Left = 564
		Width = 20
		Height = 20
		ResourceName = 'BMP_IMAGELISTTREESTATES'
	end
	object ImageList_TypeProjects: TImageList
		Left = 564
		Width = 20
		Height = 20
	end
	object TreeView_Projects: TTreeView
		Left = 0
		Top = 25
		Width = 169
		Height = 382
		Align = alLeft
		Constraints.MinWidth = 10
		Indent = 19
		MultiSelectStyle = [msControlSelect, msShiftSelect]
		StateImages = ImageList_TreeStates
		Images = ImageList_TypeProjects
		PopupMenu = PopupMenu_Projects
		RightClickSelect = True
		TabOrder = 2
	  DragMode = dmAutomatic
		OnEdited = TreeView_ProjectsEdited
		OnMouseDown = TreeView_ProjectsMouseDown
		OnDblClick = Action_EditProjectExecute
		OnDragOver = TreeView_ProjectsDragOver
		OnEndDrag = TreeView_ProjectsEndDrag
	end
	object Splitter2: TSplitter
		Left = 169
		Top = 25
		Width = 3
		Height = 382
		Cursor = crHSplit
		Align = alLeft
	end
	object PageControl_Files: TPageControl
		Left = 172
		Top = 25
		Width = 516
		Height = 382
		Align = alClient
		TabIndex = 0
		TabOrder = 0
		ShowHint = True
		Hint = ''
		DragAcceptFiles = True
		OnChange = PageControl_FilesChange
		OnMouseMove = PageControl_FilesMouseMove
		OnMouseDown = PageControl_FilesMouseDown
		OnDropFile = PageControl_FilesDropFile
	end
	object OpenDialog_WorkSpace: TOpenDialog
		Left = 592
		Top = 32
		DefaultExt = 'fvcl'
		Filter = 'Projets FreeVCL (*.fvcl)|*.fvcl|Tous (*.*)|*.*'
	end
	object OpenDialog_Project: TOpenDialog
		Left = 592
		Top = 32
		DefaultExt = 'fvcl'
		Filter = 'Projets FreeVCL (*.fvcl)|*.fvcl|Tous (*.*)|*.*'
	end
	object OpenDialog_File: TOpenDialog
		Left = 592
		Top = 32
		Filter = 'Programmes (*.c;*.cpp;*.cs;*.h)|*.c;*.cpp;*.cs;*.h|Ressources (*.rc;*.dfm)|*.rc;*.dfm|Tous (*.*)|*.*'
		Options = [ofFileMustExist, ofEnableSizing, ofAllowMultiSelect]
	end
	object OpenDialog_Dialog: TOpenDialog
		Left = 592
		Top = 32
		DefaultExt = 'dfm'
		Filter = 'Fichiers dfm|*.dfm|Fichiers rc|*.rc|Fichiers frm|*.frm|Tous (*.*)|*.*'
		Options = [ofFileMustExist, ofEnableSizing]
	end
	object OpenDialog_Graphic: TOpenDialog
		Left = 592
		Top = 32
		Filter = 'Fichiers graphiques (*.bmp;*.ico;*.cur)|*.bmp;*.ico;*.cur|Tous (*.*)|*.*'
		Options = [ofFileMustExist, ofEnableSizing]
	end
	object SaveDialog_WorkSpace: TSaveDialog
		Left = 560
		Top = 32
		DefaultExt = 'fvcl'
		Filter = 'Projets FreeVCL (*.fvcl)|*.fvcl|Tous (*.*)|*.*'
		Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
	end
	object SaveDialog_Project: TSaveDialog
		Left = 560
		Top = 32
		DefaultExt = 'fvcl'
		Filter = 'Projets FreeVCL (*.fvcl)|*.fvcl|Tous (*.*)|*.*'
		Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
	end
	object SaveDialog_File: TSaveDialog
		Left = 560
		Top = 32
		Filter = 'Programmes (*.c;*.cpp;*.cs;*.h)|*.c;*.cpp;*.cs;*.h|Ressources (*.rc;*.dfm)|*.rc;*.dfm|Tous (*.*)|*.*'
		Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
	end
	object SaveDialog_Dialog: TSaveDialog
		Left = 560
		Top = 32
		Filter = 'Fichiers dfm|*.dfm|Fichiers rc|*.rc|Tous (*.*)|*.*'
		Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
	end
	object SaveDialog_Graphics: TSaveDialog
		Left = 560
		Top = 32
		Filter = 'Fichiers dfm|*.dfm|Fichiers rc|*.rc|Tous (*.*)|*.*'
		Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
	end
	object FontDialog: TFontDialog
		Options = [fdAnsiOnly, fdFixedPitchOnly, fdForceFontExist, fdNoSimulations]
	end
	object Timer_Activate: TTimer
		Interval = 100
		Enabled = False
		OnTimer = Timer_ActivateTimer
	end
end
