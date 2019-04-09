// Copyright(c) 2019 Federico Bolelli, Costantino Grana
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
//
// *Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and / or other materials provided with the distribution.
//
// * Neither the name of THeBE nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

tree_0: if ((c+=1) >= w - 1) goto break_0_0;
		if (CONDITION_P1) {
			if (CONDITION_ITER) {
				if (CONDITION_P5) {
					if (CONDITION_P6) {
						if (CONDITION_P3) {
							ACTION_2
							goto tree_15;
						}
						else {
							if (CONDITION_P2) {
								ACTION_2
								goto tree_14;
							}
							else {
								NODE_6:
								if (CONDITION_P4) {
									ACTION_3
									goto tree_4;
								}
								else {
									ACTION_2
									goto tree_2;
								}
							}
						}
					}
					else {
						ACTION_2
						goto tree_13;
					}
				}
				else {
					if (CONDITION_P4) {
						ACTION_2
						goto tree_12;
					}
					else {
						ACTION_2
						goto tree_2;
					}
				}
			}
			else {
				if (CONDITION_P6) {
					if (CONDITION_P4) {
						ACTION_3
						goto tree_8;
					}
					else {
						ACTION_2
						goto tree_2;
					}
				}
				else {
					NODE_10:
					if (CONDITION_P4) {
						if (CONDITION_P2) {
							ACTION_3
							goto tree_6;
						}
						else {
							if (CONDITION_P5) {
								if (CONDITION_P3) {
									ACTION_3
									goto tree_5;
								}
								else {
									ACTION_2
									goto tree_4;
								}
							}
							else {
								ACTION_2
								goto tree_3;
							}
						}
					}
					else {
						ACTION_2
						goto tree_2;
					}
				}
			}
		}
		else {
			ACTION_1
			goto tree_1;
		}
tree_1: if ((c+=1) >= w - 1) goto break_0_0;
		if (CONDITION_P1) {
			if (CONDITION_ITER) {
				if (CONDITION_P5) {
					if (CONDITION_P6) {
						if (CONDITION_P3) {
							ACTION_2
							goto tree_15;
						}
						else {
							if (CONDITION_P2) {
								ACTION_2
								goto tree_14;
							}
							else {
								if (CONDITION_P9) {
									ACTION_2
									goto tree_14;
								}
								else {
									if (CONDITION_P7) {
										ACTION_3
										goto tree_14;
									}
									else{
										goto NODE_6;
									}
								}
							}
						}
					}
					else {
						ACTION_2
						goto tree_13;
					}
				}
				else {
					if (CONDITION_P4) {
						ACTION_2
						goto tree_12;
					}
					else {
						if (CONDITION_P6) {
							ACTION_2
							goto tree_2;
						}
						else {
							if (CONDITION_P7) {
								ACTION_2
								goto tree_2;
							}
							else {
								if (CONDITION_P3) {
									if (CONDITION_P2) {
										NODE_26:
										if (CONDITION_P9) {
											ACTION_3
											goto tree_2;
										}
										else {
											ACTION_2
											goto tree_2;
										}
									}
									else {
										ACTION_2
										goto tree_2;
									}
								}
								else {
									ACTION_2
									goto tree_2;
								}
							}
						}
					}
				}
			}
			else {
				if (CONDITION_P9) {
					if (CONDITION_P7) {
						ACTION_2
						goto tree_9;
					}
					else {
						if (CONDITION_P2) {
							if (CONDITION_P6) {
								if (CONDITION_P4) {
									ACTION_3
									goto tree_11;
								}
								else {
									ACTION_2
									goto tree_2;
								}
							}
							else {
								if (CONDITION_P4) {
									ACTION_3
									goto tree_6;
								}
								else {
									if (CONDITION_P5) {
										ACTION_2
										goto tree_2;
									}
									else {
										if (CONDITION_P3) {
											ACTION_3
											goto tree_2;
										}
										else {
											ACTION_2
											goto tree_2;
										}
									}
								}
							}
						}
						else {
							ACTION_2
							goto tree_10;
						}
					}
				}
				else {
					if (CONDITION_P6) {
						if (CONDITION_P4) {
							ACTION_3
							goto tree_8;
						}
						else {
							if (CONDITION_P5) {
								if (CONDITION_P7) {
									if (CONDITION_P3) {
										ACTION_2
										goto tree_2;
									}
									else {
										if (CONDITION_P2) {
											ACTION_2
											goto tree_2;
										}
										else {
											ACTION_3
											goto tree_2;
										}
									}
								}
								else {
									ACTION_2
									goto tree_2;
								}
							}
							else {
								ACTION_2
								goto tree_2;
							}
						}
					}
					else {
						if (CONDITION_P7) {
							ACTION_2
							goto tree_7;
						}
						else{
							goto NODE_10;
						}
					}
				}
			}
		}
		else {
			ACTION_1
			goto tree_1;
		}
tree_2: if ((c+=1) >= w - 1) goto break_0_1;
		ACTION_1
		goto tree_1;
tree_3: if ((c+=1) >= w - 1) goto break_0_2;
		if (CONDITION_ITER) {
			if (CONDITION_P5) {
				ACTION_2
				goto tree_13;
			}
			else {
				if (CONDITION_P4) {
					ACTION_2
					goto tree_12;
				}
				else {
					if (CONDITION_P2) {
						ACTION_3
						goto tree_2;
					}
					else {
						ACTION_2
						goto tree_2;
					}
				}
			}
		}
		else {
			ACTION_2
			goto tree_9;
		}
tree_4: if ((c+=1) >= w - 1) goto break_0_3;
		if (CONDITION_ITER) {
			NODE_45:
			if (CONDITION_P5) {
				if (CONDITION_P3) {
					ACTION_2
					goto tree_15;
				}
				else {
					if (CONDITION_P4) {
						ACTION_3
						goto tree_4;
					}
					else {
						ACTION_3
						goto tree_2;
					}
				}
			}
			else {
				if (CONDITION_P4) {
					ACTION_2
					goto tree_12;
				}
				else {
					NODE_49:
					if (CONDITION_P3) {
						ACTION_2
						goto tree_2;
					}
					else {
						ACTION_3
						goto tree_2;
					}
				}
			}
		}
		else {
			ACTION_2
			goto tree_9;
		}
tree_5: if ((c+=1) >= w - 1) goto break_0_3;
		if (CONDITION_ITER) {
			NODE_50:
			if (CONDITION_P5) {
				if (CONDITION_P3) {
					ACTION_2
					goto tree_15;
				}
				else {
					if (CONDITION_P4) {
						ACTION_2
						goto tree_4;
					}
					else {
						ACTION_3
						goto tree_2;
					}
				}
			}
			else {
				if (CONDITION_P4) {
					ACTION_2
					goto tree_12;
				}
				else {
					ACTION_3
					goto tree_2;
				}
			}
		}
		else {
			ACTION_2
			goto tree_9;
		}
tree_6: if ((c+=1) >= w - 1) goto break_0_4;
		if (CONDITION_ITER) {
			NODE_54:
			if (CONDITION_P5) {
				NODE_55:
				if (CONDITION_P6) {
					NODE_56:
					if (CONDITION_P3) {
						ACTION_2
						goto tree_15;
					}
					else {
						if (CONDITION_P4) {
							if (CONDITION_P2) {
								ACTION_2
								goto tree_4;
							}
							else {
								ACTION_3
								goto tree_4;
							}
						}
						else {
							ACTION_3
							goto tree_2;
						}
					}
				}
				else {
					ACTION_2
					goto tree_13;
				}
			}
			else {
				if (CONDITION_P4) {
					ACTION_2
					goto tree_12;
				}
				else {
					if (CONDITION_P2) {
						ACTION_3
						goto tree_2;
					}
					else {
						if (CONDITION_P3) {
							ACTION_2
							goto tree_2;
						}
						else {
							if (CONDITION_P6) {
								ACTION_3
								goto tree_2;
							}
							else {
								ACTION_2
								goto tree_2;
							}
						}
					}
				}
			}
		}
		else {
			NODE_63:
			if (CONDITION_P2) {
				if (CONDITION_P6) {
					NODE_65:
					if (CONDITION_P4) {
						ACTION_2
						goto tree_11;
					}
					else {
						ACTION_2
						goto tree_2;
					}
				}
				else {
					NODE_66:
					if (CONDITION_P4) {
						ACTION_3
						goto tree_6;
					}
					else {
						if (CONDITION_P5) {
							ACTION_2
							goto tree_2;
						}
						else {
							ACTION_3
							goto tree_2;
						}
					}
				}
			}
			else {
				ACTION_2
				goto tree_10;
			}
		}
tree_7: if ((c+=1) >= w - 1) goto break_0_5;
		if (CONDITION_P1) {
			if (CONDITION_ITER){
				goto NODE_54;
			}
			else {
				if (CONDITION_P9){
					goto NODE_63;
				}
				else {
					ACTION_2
					goto tree_9;
				}
			}
		}
		else {
			ACTION_1
			goto tree_1;
		}
tree_8: if ((c+=1) >= w - 1) goto break_0_6;
		if (CONDITION_ITER) {
			if (CONDITION_P5){
				goto NODE_55;
			}
			else {
				if (CONDITION_P4) {
					ACTION_2
					goto tree_12;
				}
				else {
					if (CONDITION_P2) {
						ACTION_3
						goto tree_2;
					}
					else {
						if (CONDITION_P3) {
							ACTION_2
							goto tree_2;
						}
						else {
							if (CONDITION_P6) {
								ACTION_3
								goto tree_2;
							}
							else{
								goto NODE_26;
							}
						}
					}
				}
			}
		}
		else {
			ACTION_2
			goto tree_9;
		}
tree_9: if ((c+=1) >= w - 1) goto break_0_7;
		if (CONDITION_P1) {
			if (CONDITION_ITER) {
				if (CONDITION_P5){
					goto NODE_55;
				}
				else {
					if (CONDITION_P4) {
						ACTION_2
						goto tree_12;
					}
					else {
						if (CONDITION_P2) {
							ACTION_3
							goto tree_2;
						}
						else {
							if (CONDITION_P3) {
								ACTION_2
								goto tree_2;
							}
							else {
								if (CONDITION_P6) {
									ACTION_3
									goto tree_2;
								}
								else {
									NODE_81:
									if (CONDITION_P7){
										goto NODE_26;
									}
									else {
										ACTION_2
										goto tree_2;
									}
								}
							}
						}
					}
				}
			}
			else {
				if (CONDITION_P9) {
					if (CONDITION_P7) {
						ACTION_2
						goto tree_9;
					}
					else{
						goto NODE_63;
					}
				}
				else {
					ACTION_2
					goto tree_9;
				}
			}
		}
		else {
			ACTION_1
			goto tree_1;
		}
tree_10: if ((c+=1) >= w - 1) goto break_0_8;
		if (CONDITION_P1) {
			if (CONDITION_ITER){
				goto NODE_54;
			}
			else {
				ACTION_2
				goto tree_9;
			}
		}
		else {
			ACTION_1
			goto tree_1;
		}
tree_11: if ((c+=1) >= w - 1) goto break_0_3;
		if (CONDITION_ITER) {
			if (CONDITION_P5){
				goto NODE_55;
			}
			else {
				NODE_86:
				if (CONDITION_P4) {
					ACTION_2
					goto tree_12;
				}
				else {
					if (CONDITION_P2) {
						ACTION_3
						goto tree_2;
					}
					else{
						goto NODE_49;
					}
				}
			}
		}
		else {
			ACTION_2
			goto tree_9;
		}
tree_12: if ((c+=1) >= w - 1) goto break_0_9;
		if (CONDITION_ITER) {
			if (CONDITION_P5) {
				ACTION_2
				goto tree_13;
			}
			else {
				if (CONDITION_P4) {
					ACTION_2
					goto tree_12;
				}
				else {
					if (CONDITION_P2) {
						ACTION_3
						goto tree_2;
					}
					else {
						if (CONDITION_P3) {
							ACTION_2
							goto tree_2;
						}
						else{
							goto NODE_81;
						}
					}
				}
			}
		}
		else {
			if (CONDITION_P9) {
				if (CONDITION_P7) {
					ACTION_2
					goto tree_9;
				}
				else {
					if (CONDITION_P2){
						goto NODE_66;
					}
					else {
						ACTION_2
						goto tree_10;
					}
				}
			}
			else {
				ACTION_2
				goto tree_9;
			}
		}
tree_13: if ((c+=1) >= w - 1) goto break_0_10;
		if (CONDITION_P1) {
			if (CONDITION_ITER) {
				if (CONDITION_P5){
					goto NODE_56;
				}
				else{
					goto NODE_86;
				}
			}
			else {
				if (CONDITION_P9) {
					if (CONDITION_P2){
						goto NODE_65;
					}
					else {
						ACTION_2
						goto tree_10;
					}
				}
				else {
					ACTION_2
					goto tree_9;
				}
			}
		}
		else {
			ACTION_1
			goto tree_1;
		}
tree_14: if ((c+=1) >= w - 1) goto break_0_10;
		if (CONDITION_P1) {
			if (CONDITION_ITER){
				goto NODE_45;
			}
			else {
				ACTION_2
				goto tree_9;
			}
		}
		else {
			ACTION_1
			goto tree_1;
		}
tree_15: if ((c+=1) >= w - 1) goto break_0_10;
		if (CONDITION_P1) {
			if (CONDITION_ITER){
				goto NODE_50;
			}
			else {
				ACTION_2
				goto tree_9;
			}
		}
		else {
			ACTION_1
			goto tree_1;
		}
break_0_0:
		if (CONDITION_P1) {
			ACTION_2
		}
		else {
			ACTION_1
		}
	continue;
break_0_1:
		ACTION_1
	continue;
break_0_2:
		if (CONDITION_ITER) {
			NODE_101:
			if (CONDITION_P2) {
				ACTION_3
			}
			else {
				ACTION_2
			}
		}
		else {
			ACTION_2
		}
	continue;
break_0_3:
		if (CONDITION_ITER) {
			ACTION_3
		}
		else {
			ACTION_2
		}
	continue;
break_0_4:
		if (CONDITION_ITER) {
			NODE_102:
			if (CONDITION_P2) {
				ACTION_3
			}
			else {
				if (CONDITION_P6) {
					ACTION_3
				}
				else {
					ACTION_2
				}
			}
		}
		else {
			NODE_104:
			if (CONDITION_P2) {
				if (CONDITION_P6) {
					ACTION_2
				}
				else {
					ACTION_3
				}
			}
			else {
				ACTION_2
			}
		}
	continue;
break_0_5:
		if (CONDITION_P1) {
			if (CONDITION_ITER){
				goto NODE_102;
			}
			else {
				if (CONDITION_P9){
					goto NODE_104;
				}
				else {
					ACTION_2
				}
			}
		}
		else {
			ACTION_1
		}
	continue;
break_0_6:
		if (CONDITION_ITER) {
			if (CONDITION_P2) {
				ACTION_3
			}
			else {
				if (CONDITION_P6) {
					ACTION_3
				}
				else {
					NODE_110:
					if (CONDITION_P9) {
						ACTION_3
					}
					else {
						ACTION_2
					}
				}
			}
		}
		else {
			ACTION_2
		}
	continue;
break_0_7:
		if (CONDITION_P1) {
			if (CONDITION_ITER) {
				if (CONDITION_P2) {
					ACTION_3
				}
				else {
					if (CONDITION_P6) {
						ACTION_3
					}
					else {
						NODE_114:
						if (CONDITION_P7){
							goto NODE_110;
						}
						else {
							ACTION_2
						}
					}
				}
			}
			else {
				if (CONDITION_P9) {
					if (CONDITION_P7) {
						ACTION_2
					}
					else{
						goto NODE_104;
					}
				}
				else {
					ACTION_2
				}
			}
		}
		else {
			ACTION_1
		}
	continue;
break_0_8:
		if (CONDITION_P1) {
			if (CONDITION_ITER){
				goto NODE_102;
			}
			else {
				ACTION_2
			}
		}
		else {
			ACTION_1
		}
	continue;
break_0_9:
		if (CONDITION_ITER) {
			if (CONDITION_P2) {
				ACTION_3
			}
			else{
				goto NODE_114;
			}
		}
		else {
			if (CONDITION_P9) {
				if (CONDITION_P7) {
					ACTION_2
				}
				else{
					goto NODE_101;
				}
			}
			else {
				ACTION_2
			}
		}
	continue;
break_0_10:
		if (CONDITION_P1) {
			if (CONDITION_ITER) {
				ACTION_3
			}
			else {
				ACTION_2
			}
		}
		else {
			ACTION_1
		}
	continue;
