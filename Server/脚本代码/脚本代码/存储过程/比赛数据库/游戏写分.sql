
----------------------------------------------------------------------------------------------------

USE QPGameMatchDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_WriteGameScore]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_WriteGameScore]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- 游戏写分
CREATE PROC GSP_GR_WriteGameScore

	-- 系统信息
	@dwUserID INT,								-- 用户 I D
	@dwDBQuestID INT,							-- 请求标识
	@dwInoutIndex INT,							-- 进出索引

	-- 变更成绩
	@lVariationScore BIGINT,					-- 用户分数
	@lVariationGrade BIGINT,					-- 用户成绩
	@lVariationInsure BIGINT,					-- 用户银行
	@lVariationRevenue BIGINT,					-- 游戏税收
	@lVariationWinCount INT,					-- 胜利盘数
	@lVariationLostCount INT,					-- 失败盘数
	@lVariationDrawCount INT,					-- 和局盘数
	@lVariationFleeCount INT,					-- 断线数目
	@lVariationUserMedal INT,					-- 用户奖牌
	@lVariationExperience INT,					-- 用户经验
	@lVariationLoveLiness INT,					-- 用户魅力
	@dwVariationPlayTimeCount INT,				-- 游戏时间

	-- 附件信息
	--@cbTaskForward TINYINT,						-- 任务跟进

	-- 比赛信息
	@dwMatchID INT,							    -- 比赛 I D
	@dwMatchNO INT,								-- 比赛场次

	-- 属性信息
	@wKindID INT,								-- 游戏 I D
	@wServerID INT,								-- 房间 I D
	@strClientIP NVARCHAR(15)					-- 连接地址

WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 比赛分数
	UPDATE MatchScoreInfo SET Score=Score+@lVariationScore, WinCount=WinCount+@lVariationWinCount, LostCount=LostCount+@lVariationLostCount,
		DrawCount=DrawCount+@lVariationDrawCount, FleeCount=FleeCount+@lVariationFleeCount, PlayTimeCount=PlayTimeCount+@dwVariationPlayTimeCount
	WHERE UserID=@dwUserID AND ServerID=@wServerID AND MatchID=@dwMatchID AND MatchNo=@dwMatchNo
	
	DECLARE @TreasureScore BIGINT
	SELECT @TreasureScore=Score+@lVariationScore FROM QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo WHERE UserID=@dwUserID
	
	if @TreasureScore < 0
	BEGIN							
		SET @TreasureScore=0
	END	
	
	-- 用户积分
	UPDATE QPTreasureDBLink.QPTreasureDB.dbo.GameScoreInfo SET Score=@TreasureScore,
		WinCount=WinCount+@lVariationWinCount, LostCount=LostCount+@lVariationLostCount, DrawCount=DrawCount+@lVariationDrawCount,
		FleeCount=FleeCount+@lVariationFleeCount, PlayTimeCount=PlayTimeCount+@dwVariationPlayTimeCount
	WHERE UserID=@dwUserID

	-- 游戏时长
	UPDATE GameScoreInfo SET PlayTimeCount=PlayTimeCount+@dwVariationPlayTimeCount WHERE UserID=@dwUserID	

	-- 全局信息
	IF @lVariationExperience>0 OR @lVariationLoveLiness<>0 OR @lVariationUserMedal>0
	BEGIN
		UPDATE QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo SET Experience=Experience+@lVariationExperience, LoveLiness=LoveLiness+@lVariationLoveLiness,
			UserMedal=UserMedal+@lVariationUserMedal
		WHERE UserID=@dwUserID
	END
	
	-- 任务跟进
--	IF @cbTaskForward=1
--	BEGIN
--		exec QPPlatformDBLink.QPPlatformDB.dbo.GSP_GR_TaskForward @dwUserID,@wKindID,0,@lVariationWinCount,@lVariationLostCount,@lVariationDrawCount	
--	END

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------