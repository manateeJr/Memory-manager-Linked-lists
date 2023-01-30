
#include "MemoryManager.h"
#include "MList.h"
#include "gtest/gtest.h"

class metaTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
  }
 
  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

/* A simple example of Unit Test */
TEST_F(metaTest,testClaim)
{
  MemoryManager *mm = new MemoryManager();
  // Execute once the code is implemented 
  
  int ret = mm->claimMemory(20);
  EXPECT_EQ(mm->memStart, ret);
  EXPECT_EQ(mm->memStart, mm->allocList->head->_addr);
  EXPECT_EQ(20, mm->allocList->head->_size);
  delete mm;
}

TEST_F(metaTest, testFree) {
	MemoryManager *mm = new MemoryManager(); 
	mm->claimMemory(20);
	mm->claimMemory(10);
	mm->claimMemory(5);
	mm->claimMemory(10);
	
	EXPECT_EQ(20, mm->freeMemory(1000));
	EXPECT_EQ(10, mm->freeMemory(1020));
	EXPECT_EQ(5, mm->freeMemory(1030)); 
	EXPECT_EQ(10, mm->freeMemory(1035)); 
	
	mm->claimMemory(8);
	mm->claimMemory(10);
	mm->claimMemory(12);
	mm->claimMemory(14);
	mm->claimMemory(3);
	
	EXPECT_EQ(8, mm->freeMemory(1000));
	EXPECT_EQ(10, mm->freeMemory(1008)); 
	EXPECT_EQ(12, mm->freeMemory(1018)); 
	EXPECT_EQ(14, mm->freeMemory(1030));
	EXPECT_EQ(3, mm->freeMemory(1044));

	delete mm;
}




