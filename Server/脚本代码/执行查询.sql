USE [QPPlatformDB]
GO

DECLARE	@return_value int

EXEC	@return_value = [dbo].[GSP_GR_LoadPrivateInfo]
		@wKindID = 27

SELECT	'Return Value' = @return_value

GO
