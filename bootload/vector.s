.section .vector_soft_intr,"ax",@progbits
.set noreorder
.ent _vector_soft_intr
_vector_soft_intr:
  di
  mfc0  $k1, $13
  andi  $k0, $k1,(0x08 << 2)
  beq $k0,$0,intr_softerr
  nop
  j intr_syscall
  nop
.end _vector_soft_intr

# タイマ0
.section .vector_8,"ax",@progbits
.set noreorder
.ent _vector_8
_vector_8:
  di
  j intr_tmrintr
  nop

# タイマ1
.section .vector_12,"ax",@progbits
.set noreorder
.ent _vector_12
_vector_12:
  di
  j intr_tmrintr
  nop

# タイマ2
.section .vector_16,"ax",@progbits
.set noreorder
.ent _vector_16
_vector_16:
  di
  j intr_tmrintr
  nop

# タイマ3
.section .vector_20,"ax",@progbits
.set noreorder
.ent _vector_20
_vector_20:
  di
  j intr_tmrintr
  nop

.section .vector_37,"ax",@progbits
.set noreorder
.ent _vector_37
_vector_37:
  di
  j intr_serintr
  nop
.end _vector_37
